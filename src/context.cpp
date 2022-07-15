#include "context.h"

#include "function.h"
#include "native_function.h"
#include "object.h"
#include "value.h"

#include "_std.h"
#include "_std_file.h"
#include "_std_list.h"
#include "_std_math.h"
#include "_std_string.h"

#define SCOPE_SWEEP 1000

namespace nouse {
Value* _getVarCurrentScope(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  return ctx->getVariableCurrentScope(args[0]->getString()->value, branch - 1);
}

NativeFunction* nfGetVarCurrentScope;
Value* vGetVarCurrentScope;
void initContext() {
  nfGetVarCurrentScope = new NativeFunction(&_getVarCurrentScope, b_false);
  vGetVarCurrentScope = new Value();
  vGetVarCurrentScope->setNativeFunction(nfGetVarCurrentScope);
}
void closeContext() {
  delete vGetVarCurrentScope;
  delete nfGetVarCurrentScope;
}
Value* getVarCurrentScope() { return vGetVarCurrentScope; }

Context::Context(i64 argc, char** argv) {
  this->scopeSweep_ = SCOPE_SWEEP;

  this->globalVariables = new Object();

  // _std.h
  this->registerFunction("this", &_getThis);

  this->registerFunction("set_show_errors", &_setShowErrors);
  this->registerFunction("set_show_user_errors", &_setShowUserErrors);
  this->registerFunction("delete", &_deleteObj);
  this->registerFunction("args", &_args);
  this->registerFunction("args_assert", &_argsAssert);
  this->registerFunction("print", &_print);
  this->registerFunction("println", &_printLine);
  this->registerFunction("input", &_input);
  this->registerFunction("type", &_getType);
  this->registerFunction("obj", &_newObject);
  this->registerFunction("tuple", &_newTuple);
  this->registerFunction("tuple_get", &_tupleGet);
  this->registerFunction("error", &_createError);
  this->registerFunction("string", &_toString);
  this->registerFunction("int", &_toInt);
  this->registerFunction("float", &_toFloat);
  this->registerFunction("if", &_extIf);
  this->registerFunction("time", &_time);
  this->registerFunction("require", &_require);
  this->registerFunction("compile_string", &_compileString);
  this->registerFunction("$", &_arithm);
  this->registerFunction("get_command_line_args", &_getCommandlineArgs);

  // _std_string.h
  this->registerFunction("format", &_format);
  this->registerFunction("str_length", &_stringLength);
  this->registerFunction("str_char_at", &_stringCharAt);
  this->registerFunction("str_substr", &_stringSubstr);
  this->registerFunction("str_find", &_stringFind);
  this->registerFunction("str_find_char", &_stringFindChar);

  // _std_math.h
  this->setGlobal("math", getStdMathObj());

  // _std_list.h
  this->registerFunction("list", &_newList);

  // _std_file.h
  this->registerFunction("file_read", &_readFile);
  this->registerFunction("file_write", &_writeFile);

  for (i64 i = 1; i < argc; i++) {
    this->commandLineArgs.push_back(argv[i]);
  }
}
Context::~Context() {
  for (int i = 0; i < this->objects.size(); i++) {
    if (!this->objects[i]->deleted) {
      if (this->objects[i]->hasField("__del")) {
        Value* destructor = this->objects[i]->getField("__del");
        if (destructor->getType() == ValueType::NATIVE_FUNCTION ||
            destructor->getType() == ValueType::FUNCTION) {
          delete this->objects[i]->callMethod(this, "__del");
        }
      }
      if (this->objects[i]->hasField(";__del")) {
        Value* destructor = this->objects[i]->getField(";__del");
        if (destructor->getType() == ValueType::NATIVE_FUNCTION ||
            destructor->getType() == ValueType::FUNCTION) {
          delete this->objects[i]->callMethod(this, ";__del");
        }
      }
    }
  }
  for (int i = 0; i < this->objects.size(); i++) {
    delete this->objects[i];
  }
  delete this->globalVariables;
  for (int i = 0; i < this->variables.size(); i++) {
    for (int j = 0; j < this->variables[i].size(); j++) {
      delete this->variables[i][j];
    }
  }
  for (int i = 0; i < this->functions.size(); i++) {
    delete this->functions[i];
  }
  for (int i = 0; i < this->nativeFunctions.size(); i++) {
    delete this->nativeFunctions[i];
  }
}

void Context::newObject(Object* o) { this->objects.push_back(o); }
void Context::deleteObject(Object* o) {
  std::map< stdstr_t, Value* >::iterator it;
  for (it = this->globalVariables->fields.begin();
       it != this->globalVariables->fields.end();
       it++) {
    if (it->second->getType() == ValueType::OBJECT) {
      if (it->second->getObject() == o) {
        it->second->makeNone();
      }
    }
  }
  for (i64 i = 0; i < this->variables.size(); i++) {
    for (i64 j = 0; j < this->variables[i].size(); j++) {
      for (it = this->variables[i][j]->fields.begin();
           it != this->variables[i][j]->fields.end();
           it++) {
        if (it->second->getType() == ValueType::OBJECT) {
          if (it->second->getObject() == o) {
            it->second->makeNone();
          }
        }
      }
    }
  }
  for (i64 i = 0; i < this->objects.size(); i++) {
    if (objects[i] == o) {
      objects.erase(objects.begin() + i);
      delete o;
    }
  }
}

Value* Context::getVariable(stdstr_t name, i64 branch) {
  // std::cout << "looking for " << name << " on branch " << branch <<
  // std::endl;

  if (branch != -1) {
    for (i64 i = this->variables[branch].size() - 1; i >= 0; i--) {
      if (this->variables[branch][i]->hasField(name)) {
        return this->variables[branch][i]->getField(name);
      }
    }
  }

  if (branch != 0) {
    for (i64 i = 0; i < this->variables[0].size(); i++) {
      if (this->variables[0][i]->hasField(name))
        return this->variables[0][i]->getField(name);
    }
  }
  if (this->globalVariables->hasField(name))
    return this->globalVariables->getField(name);

  Value* v = new Value();
  Value* a = this->setVariable(name, v, branch);
  delete v;
  return a;
}

Value* Context::getVariableCurrentScope(stdstr_t name, i64 branch) {
  if (branch == -1) {
    return this->globalVariables->getField(name);
  }
  return this->variables[branch][this->variables[branch].size() - 1]->getField(
      name);
}

Value* Context::setGlobal(stdstr_t name, Value* v) {
  return this->globalVariables->setField(name, v);
}
Value* Context::setVariable(stdstr_t name, Value* v, i64 branch, i64 scope) {
  if (branch == -1) {
    return this->setGlobal(name, v);
  }
  if (scope == -1) {
    scope = this->variables[branch].size() - 1;
  }
  return this->variables[branch][scope]->setField(name, v);
}
i64 Context::pushBranch() {
  this->variables.push_back(std::vector< Object* >());
  this->variables[this->variables.size() - 1].push_back(new Object());
  this->scopeDisposable.push_back(std::vector< boolean >());
  this->scopeDisposable[this->variables.size() - 1].push_back(b_true);
  return this->variables.size() - 1;
}
void Context::popBranch() {
  for (i64 i = 0; i < this->variables[this->variables.size() - 1].size(); i++) {
    if (this->scopeDisposable[this->variables.size() - 1][i])
      delete this->variables[this->variables.size() - 1][i];
  }
  this->variables.pop_back();
  this->scopeDisposable.pop_back();
  // this->collectGarbage();
  // this->scopeSweep_ = 10;
}
i64 Context::branchPushScope(i64 branch, Object* obj) {
  if (obj == nullptr) {
    this->variables[branch].push_back(new Object());
    this->scopeDisposable[branch].push_back(b_true);
  } else {
    this->variables[branch].push_back(obj);
    this->scopeDisposable[branch].push_back(b_false);
  }
  return this->variables[branch].size() - 1;
}
void Context::branchPopScope(i64 branch) {
  if (this->scopeDisposable[branch][this->variables[branch].size() - 1])
    delete this->variables[branch][this->variables[branch].size() - 1];
  this->variables[branch].pop_back();
  this->scopeDisposable[branch].pop_back();
  this->scopeSweep_--;
  if (this->scopeSweep_ <= 0) {
    this->collectGarbage();
    this->scopeSweep_ = SCOPE_SWEEP;
  }
}

void Context::collectGarbage() {
  for (i64 i = 0; i < this->objects.size(); i++) {
    if (this->objects[i]->getRefCount() <= 0) {
      if (!this->objects[i]->deleted) {
        if (this->objects[i]->hasField("__del")) {
          Value* destructor = this->objects[i]->getField("__del");
          if (destructor->getType() == ValueType::NATIVE_FUNCTION ||
              destructor->getType() == ValueType::FUNCTION) {
            delete this->objects[i]->callMethod(this, "__del");
          }
        }
        if (this->objects[i]->hasField(";__del")) {
          Value* destructor = this->objects[i]->getField(";__del");
          if (destructor->getType() == ValueType::NATIVE_FUNCTION ||
              destructor->getType() == ValueType::FUNCTION) {
            delete this->objects[i]->callMethod(this, ";__del");
          }
        }
      }
      delete this->objects[i];
      this->objects.erase(this->objects.begin() + i);
      i--;
    }
  }
  for (i64 i = 0; i < this->functions.size(); i++) {
    if (this->functions[i]->getRefCount() <= 0) {
      delete functions[i];
      functions.erase(functions.begin() + i);
      i--;
    }
  }
}

void Context::registerFunction(stdstr_t name,
    Value* (*f)(Context* ctx, i64 branch, i64 line, i64 fileind),
    boolean resultDisposable) {
  NativeFunction* nouseF = new NativeFunction(f, resultDisposable);
  nativeFunctions.push_back(nouseF);
  Value* v = new Value();
  v->setNativeFunction(nouseF);
  this->setGlobal(name, v);
}

void Context::trackFunction(Function* f) { this->functions.push_back(f); }

void Context::callFunction(stdstr_t name) {
  Value* f = this->getVariable(name);
  if (f->getType() == ValueType::FUNCTION) {
    Function* fun = f->getFunction();
    delete fun->call(this, this->pushBranch());
    this->popBranch();
  } else if (f->getType() == ValueType::NATIVE_FUNCTION) {
    NativeFunction* fun = f->getNativeFunction();
    delete fun->call(this, this->pushBranch());
    this->popBranch();
  } else {
    // error probably
  }
}

void Context::pushFunctionArgs(std::vector< Value* >& args) {
  this->functionArguments.push_back(args);
}
std::vector< Value* >& Context::getTopFunctionArgs(i64 ind) {
  return this->functionArguments[this->functionArguments.size() - 1 - ind];
}
void Context::popFunctionArgs() { this->functionArguments.pop_back(); }

i64 Context::getNBranches() { return variables.size(); }
} // namespace nouse
