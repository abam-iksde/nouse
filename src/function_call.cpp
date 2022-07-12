#include "function_call.h"

#include "context.h"
#include "debug.h"
#include "function.h"
#include "native_function.h"
#include "string.h"
#include "tokenize.h"
#include "value.h"

namespace nouse {
FunctionCall::FunctionCall(Value* f,
    std::vector< Value* > arg,
    i64 line_,
    i64 fileind_,
    boolean appendBranch_) {
  this->function = f;
  this->args = arg;
  this->line = line_;
  this->fileind = fileind_;
  this->appendBranch = appendBranch_;
}
FunctionCall::~FunctionCall() {
  delete this->function;
  for (i64 i = 0; i < this->args.size(); i++) {
    delete this->args[i];
  }
}
Value* FunctionCall::execute(Context* ctx, i64 branch) {
  Value* f = this->function;
  boolean delf = b_false;
  if (f->getType() == ValueType::NAME)
    f = ctx->getVariable(f->getName(), branch - 1);
  else if (f->getType() == ValueType::FUNCTION_CALL) {
    FunctionCall* _f = f->getFunctionCall();
    f = _f->execute(ctx, branch);
    ctx->popBranch();
    ctx->pushBranch();
    delf = b_true;
    if (_f->function->getType() == ValueType::NATIVE_FUNCTION) {
      delf = _f->function->getNativeFunction()->isResultDisposable();
    }
  }
  std::vector< boolean > argVarRefs;
  std::vector< Value* > args_;
  for (int i = 0; i < args.size(); i++) {
    Value* a = this->args[i];
    if (a->getType() == ValueType::NAME) {
      a = ctx->getVariable(a->getName(), branch - 1);
      argVarRefs.push_back(b_true);
    } else if (a->getType() == ValueType::FUNCTION_CALL) {
      Value* vFunction = a->getFunctionCall()->function;
      a = a->getFunctionCall()->execute(ctx, branch);
      ctx->popBranch();
      ctx->pushBranch();
      if (vFunction->getType() == ValueType::NATIVE_FUNCTION) {
        argVarRefs.push_back(
            !vFunction->getNativeFunction()->isResultDisposable());
      } else {
        argVarRefs.push_back(b_false);
      }
    } else {
      Value* v_ = new Value();
      v_->set(a);
      a = v_;
      argVarRefs.push_back(b_false);
    }
    args_.push_back(a);
  }
  ctx->pushFunctionArgs(args_);
  Value* result = new Value();
  if (f->getType() == ValueType::FUNCTION) {
    delete result;

    Function* fun = f->getFunction();
    result = fun->call(ctx, branch);
  } else if (f->getType() == ValueType::NATIVE_FUNCTION) {
    delete result;

    NativeFunction* fun = f->getNativeFunction();
    result = fun->call(ctx, branch, this->line, this->fileind);
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(this->fileind)
                << "' line: " << line << ": attepting to call a value of type "
                << getValueTypeName(f->getType()) << std::endl;
    result = new Value();
    String* s = new String("InvalidFunction");
    result->setError(s);
    delete s;
  }

  ctx->popFunctionArgs();
  if (delf)
    delete f;
  for (i64 i = 0; i < args_.size(); i++) {
    if (!argVarRefs[i])
      delete args_[i];
  }
  return result;
}
} // namespace nouse
