#include "object.h"

#include <cmath>

#include "debug.h"

#include "function.h"
#include "string.h"
#include "tokenize.h"
#include "tuple.h"

namespace nouse {
Value* operator_IsEqual(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    if (args[0]->getType() == ValueType::INT &&
        args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getInt() == args[1]->getInt());
    } else if (args[0]->getType() == ValueType::INT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getInt() == args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getFloat() == args[1]->getInt());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getFloat() == args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::BOOLEAN &&
               args[1]->getType() == ValueType::BOOLEAN) {
      result->setBoolean(args[0]->getBoolean() == args[1]->getBoolean());
    } else if (args[0]->getType() == ValueType::STRING &&
               args[1]->getType() == ValueType::STRING) {
      result->setBoolean(
          args[0]->getString()->value == args[1]->getString()->value);
    } else if (args[0]->getType() == ValueType::_ERROR &&
               args[1]->getType() == ValueType::STRING) {
      result->setBoolean(
          args[0]->getString()->value == args[1]->getString()->value);
    } else if (args[0]->getType() == ValueType::STRING &&
               args[1]->getType() == ValueType::_ERROR) {
      result->setBoolean(
          args[0]->getString()->value == args[1]->getString()->value);
    } else if (args[0]->getType() == ValueType::_ERROR &&
               args[1]->getType() == ValueType::_ERROR) {
      result->setBoolean(
          args[0]->getString()->value == args[1]->getString()->value);
    } else {
      result->setBoolean(args[0]->getV() == args[1]->getV());
    }
  }
  return result;
}
Value* operator_IsNotEqual(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  Value* v = operator_IsEqual(ctx, branch, line, fileind);
  result->setBoolean(!v->getBoolean());
  delete v;
  return result;
}
Value* operator_IsSmaller(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    if (args[0]->getType() == ValueType::INT &&
        args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getInt() < args[1]->getInt());
    } else if (args[0]->getType() == ValueType::INT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getInt() < args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getFloat() < args[1]->getInt());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getFloat() < args[1]->getFloat());
    }
  }
  return result;
}
Value* operator_IsSmallerOrEqual(
    Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    if (args[0]->getType() == ValueType::INT &&
        args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getInt() <= args[1]->getInt());
    } else if (args[0]->getType() == ValueType::INT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getInt() <= args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getFloat() <= args[1]->getInt());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getFloat() <= args[1]->getFloat());
    }
  }
  return result;
}
Value* operator_IsGreater(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    if (args[0]->getType() == ValueType::INT &&
        args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getInt() > args[1]->getInt());
    } else if (args[0]->getType() == ValueType::INT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getInt() > args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getFloat() > args[1]->getInt());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getFloat() > args[1]->getFloat());
    }
  }
  return result;
}
Value* operator_IsGreaterOrEqual(
    Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    if (args[0]->getType() == ValueType::INT &&
        args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getInt() >= args[1]->getInt());
    } else if (args[0]->getType() == ValueType::INT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getInt() >= args[1]->getFloat());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::INT) {
      result->setBoolean(args[0]->getFloat() >= args[1]->getInt());
    } else if (args[0]->getType() == ValueType::FLOAT &&
               args[1]->getType() == ValueType::FLOAT) {
      result->setBoolean(args[0]->getFloat() >= args[1]->getFloat());
    }
  }
  return result;
}
Value* operator_Not(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  Value* result = new Value();
  if (args.size() > 0) {
    result->setBoolean(!args[0]->getBoolean());
  }
  return result;
}
Value* operator_And(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  Value* result = new Value();
  result->setBoolean(b_true);
  for (i64 i = 0; i < args.size(); i++) {
    if (!args[i]->getBoolean()) {
      result->setBoolean(b_false);
      return result;
    }
  }
  return result;
}
Value* operator_Or(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  Value* result = new Value();
  result->setBoolean(b_false);
  for (i64 i = 0; i < args.size(); i++) {
    if (args[i]->getBoolean()) {
      result->set(args[i]);
      return result;
    }
  }
  return result;
}

Value* operator_Set(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  args[0]->set(args[1]);

  Value* ret = new Value();
  ret->set(args[0]);
  return ret;
}
Value* operator_Add(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();

  if (args[0]->getType() == ValueType::INT &&
      args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setInt(args[0]->getInt() + args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() + args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getInt() + args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() + args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::STRING &&
             args[1]->getType() == ValueType::STRING) {
    Value* result = new Value();
    String* _s =
        new String(args[0]->getString()->value + args[1]->getString()->value);
    result->setString(_s);
    delete _s;
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": cannot add two values of types '"
                << getValueTypeName(args[0]->getType()) << "' and '"
                << getValueTypeName(args[1]->getType()) << '\'' << std::endl;
    Value* result = new Value();
    String* _s = new String("CalcTypeMismatch");
    result->setError(_s);
    delete _s;
    return result;
  }

  return new Value();
}

Value* operator_Sub(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();

  if (args[0]->getType() == ValueType::INT &&
      args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setInt(args[0]->getInt() - args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() - args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getInt() - args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() - args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::NONE &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setInt(-args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::NONE &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(-args[1]->getFloat());
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line
                << ": cannot subtract two values of types '"
                << getValueTypeName(args[0]->getType()) << "' and '"
                << getValueTypeName(args[1]->getType()) << '\'' << std::endl;
    Value* result = new Value();
    String* _s = new String("CalcTypeMismatch");
    result->setError(_s);
    delete _s;
    return result;
  }

  return new Value();
}

Value* operator_Mult(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();

  if (args[0]->getType() == ValueType::INT &&
      args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setInt(args[0]->getInt() * args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() * args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getInt() * args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() * args[1]->getInt());
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line
                << ": cannot add multiply values of types '"
                << getValueTypeName(args[0]->getType()) << "' and '"
                << getValueTypeName(args[1]->getType()) << '\'' << std::endl;
    Value* result = new Value();
    String* _s = new String("CalcTypeMismatch");
    result->setError(_s);
    delete _s;
    return result;
  }

  return new Value();
}

Value* operator_Div(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();

  if (args[0]->getType() == ValueType::INT &&
      args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat((f64)args[0]->getInt() / (f64)args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() / args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(args[0]->getInt() / args[1]->getFloat());
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat(args[0]->getFloat() / args[1]->getInt());
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": cannot add divide values of types '"
                << getValueTypeName(args[0]->getType()) << "' and '"
                << getValueTypeName(args[1]->getType()) << '\'' << std::endl;
    Value* result = new Value();
    String* _s = new String("CalcTypeMismatch");
    result->setError(_s);
    delete _s;
    return result;
  }

  return new Value();
}

Value* _format(Context* ctx, i64 branch, i64 line, i64 fileind);
Value* operator_Modulo(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args[0]->getType() == ValueType::STRING) {
    return _format(ctx, branch, line, fileind);
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setInt(args[0]->getInt() % args[1]->getInt());
    return result;
  } else if (args[0]->getType() == ValueType::INT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(std::fmod((f64)args[0]->getInt(), args[1]->getFloat()));
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::INT) {
    Value* result = new Value();
    result->setFloat(std::fmod(args[0]->getFloat(), (f64)args[1]->getInt()));
    return result;
  } else if (args[0]->getType() == ValueType::FLOAT &&
             args[1]->getType() == ValueType::FLOAT) {
    Value* result = new Value();
    result->setFloat(std::fmod(args[0]->getFloat(), args[1]->getFloat()));
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": cannot perform '%' operation with '"
                << getValueTypeName(args[0]->getType()) << "' and '"
                << getValueTypeName(args[1]->getType()) << '\'' << std::endl;
    Value* result = new Value();
    String* _s = new String("InvalidArgument");
    result->setError(_s);
    delete _s;
    return result;
  }
}

Value* operator_ObjGet(Context* ctx, i64 branch, i64 line, i64 fileind) {
  return _objectGet(ctx, branch, line, fileind);
}

Value* operator_If(Context* ctx, i64 branch, i64 line, i64 fileind) {
  branch--;
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  Value* result = new Value();
  if (args.size() == 2) {
    if (args[0]->getBoolean()) {
      if (args[1]->getType() == ValueType::TUPLE) {
        if (args[1]->getTuple()->length() > 0) {
          result->set(args[1]->getTuple()->data()[0]);
        }
      } else {
        result->set(args[1]);
      }
    } else {
      if (args[1]->getType() == ValueType::TUPLE) {
        if (args[1]->getType() == ValueType::TUPLE) {
          if (args[1]->getTuple()->length() > 1) {
            result->set(args[1]->getTuple()->data()[1]);
          }
        }
      }
    }
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line
                << ": second argument of '?' operator has to be function or "
                   "(fun_if_b_true, fun_if_b_false) tuple, got "
                << getValueTypeName(args[1]->getType()) << std::endl;
    String* _s = new String("InvalidIfBody");
    result->setError(_s);
    delete _s;
    return result;
  }

  ctx->popBranch();
  ctx->branchPushScope(branch);
  if (result->getType() == ValueType::FUNCTION) {
    Value* f = result;
    result = f->getFunction()->call(ctx, branch);
    delete f;
  } else if (result->getType() == ValueType::NATIVE_FUNCTION) {
    Value* f = result;
    Value* t = f->getNativeFunction()->call(ctx, branch, line);
    result = new Value();
    result->set(t);
    if (f->getNativeFunction()->isResultDisposable())
      delete t;
    delete f;
  }
  ctx->branchPopScope(branch);
  ctx->pushBranch();

  return result;
}
Value* operator_Do(Context* ctx, i64 branch, i64 line, i64 fileind) {
  branch--;
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args.size() == 2) {
    Value* cond = args[0];
    if (cond->getType() == ValueType::FUNCTION) {
      Value* fun = args[1];
      if (fun->getType() != ValueType::FUNCTION &&
          fun->getType() != ValueType::NATIVE_FUNCTION) {
        if (showErrors())
          std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                    << "' line: " << line
                    << ": second argument of 'do' operator has to be a "
                       "function or a native function"
                    << std::endl;
        Value* result = new Value();
        String* _s = new String("InvalidDoBody");
        result->setError(_s);
        delete _s;
        return result;
      }

      ctx->popBranch();

      ctx->branchPushScope(branch);
      Value* vCond = cond->getFunction()->call(ctx, branch);
      ctx->branchPopScope(branch);
      while (vCond->getBoolean()) {
        ctx->branchPushScope(branch);
        if (fun->getType() == ValueType::FUNCTION) {
          delete fun->getFunction()->call(ctx, branch);
        } else if (fun->getType() == ValueType::NATIVE_FUNCTION) {
          delete fun->getNativeFunction()->call(ctx, branch, line);
        }
        ctx->branchPopScope(branch);

        delete vCond;
        ctx->branchPushScope(branch);
        vCond = cond->getFunction()->call(ctx, branch);
        ctx->branchPopScope(branch);
      }
      delete vCond;

      ctx->pushBranch();
    } else if (cond->getType() == ValueType::NONE) {
      Value* fun = args[1];
      if (fun->getType() != ValueType::FUNCTION &&
          fun->getType() != ValueType::NATIVE_FUNCTION) {
        if (showErrors())
          std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                    << "' line: " << line
                    << ": second argument of 'do' operator has to be a "
                       "function or a native function"
                    << std::endl;
        Value* result = new Value();
        String* _s = new String("InvalidDoBody");
        result->setError(_s);
        delete _s;
        return result;
      }

      Value* result = nullptr;
      ctx->branchPushScope(branch);
      if (fun->getType() == ValueType::FUNCTION) {
        result = fun->getFunction()->call(ctx, branch);
      } else if (fun->getType() == ValueType::NATIVE_FUNCTION) {
        result = fun->getNativeFunction()->call(ctx, branch, line);
      }
      ctx->branchPopScope(branch);

      return result;
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": first argument of 'do' operator has to be a function "
                     "or a none-type"
                  << std::endl;
      Value* result = new Value();
      String* _s = new String("InvalidDoCondition");
      result->setError(_s);
      delete _s;
      return result;
    }
  }
  return new Value();
}

Value* operator_With(Context* ctx, i64 branch, i64 line, i64 fileind) {
  branch--;
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() >= 2) {
    if (args[0]->getType() != ValueType::OBJECT) {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": first argument of '->' has to be an object"
                  << std::endl;
      Value* result = new Value();
      String* _s = new String("InvalidArgument");
      result->setError(_s);
      delete _s;
      return result;
    } else if (args[1]->getType() != ValueType::FUNCTION) {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": second argument of '->' has to be a function"
                  << std::endl;
      Value* result = new Value();
      String* _s = new String("InvalidArgument");
      result->setError(_s);
      delete _s;
      return result;
    } else {
      ctx->popBranch();
      ctx->branchPushScope(branch, args[0]->getObject());
      delete args[1]->getFunction()->call(ctx, branch);
      ctx->branchPopScope(branch);
      ctx->pushBranch();
    }
  }
  Value* result = new Value();
  Object* ref = args[0]->getObject()->getRef();
  result->setObject(ref);
  ref->decRef();
  return result;
}

Value* operator_DoInCurrentBranch(
    Context* ctx, i64 branch, i64 line, i64 fileind) {
  branch--;
  Value* result = nullptr;
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() >= 2) {
    std::vector< Value* > arguments;
    if (args[1]->getType() == ValueType::TUPLE) {
      ctx->pushFunctionArgs(args[1]->getTuple()->data());
    } else {
      arguments.push_back(args[1]);
      ctx->pushFunctionArgs(arguments);
    }
    if (args[0]->getType() == ValueType::FUNCTION) {
      Function* f = args[0]->getFunction();
      ctx->popBranch();
      ctx->branchPushScope(branch);
      result = f->call(ctx, branch);
      ctx->branchPopScope(branch);
      ctx->pushBranch();
    } else if (args[0]->getType() == ValueType::NATIVE_FUNCTION) {
      NativeFunction* f = args[0]->getNativeFunction();
      result = f->call(ctx, branch);
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line << ": trying to call a value of type '"
                  << getValueTypeName(args[0]->getType()) << '\'' << std::endl;
      result = new Value();
      String* s = new String("InvalidFunctionCall");
      result->setError(s);
      delete s;
      ctx->popFunctionArgs();
      return result;
    }
    ctx->popFunctionArgs();
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": malformed '@' function call"
                << std::endl;
  }
  return result;
}

Value* operator_DoInCurrentScope(
    Context* ctx, i64 branch, i64 line, i64 fileind) {
  branch--;
  Value* result = nullptr;
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() >= 2) {
    std::vector< Value* > arguments;
    if (args[1]->getType() == ValueType::TUPLE) {
      ctx->pushFunctionArgs(args[1]->getTuple()->data());
    } else {
      arguments.push_back(args[1]);
      ctx->pushFunctionArgs(arguments);
    }
    if (args[0]->getType() == ValueType::FUNCTION) {
      Function* f = args[0]->getFunction();
      ctx->popBranch();
      result = f->call(ctx, branch);
      ctx->pushBranch();
    } else if (args[0]->getType() == ValueType::NATIVE_FUNCTION) {
      NativeFunction* f = args[0]->getNativeFunction();
      result = f->call(ctx, branch);
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line << ": trying to call a value of type '"
                  << getValueTypeName(args[0]->getType()) << '\'' << std::endl;
      result = new Value();
      String* s = new String("InvalidFunctionCall");
      result->setError(s);
      delete s;
      ctx->popFunctionArgs();
      return result;
    }
    ctx->popFunctionArgs();
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": malformed '@' function call"
                << std::endl;
  }
  return result;
}
} // namespace nouse
