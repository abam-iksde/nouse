#include "_std_math.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "context.h"
#include "debug.h"
#include "native_function.h"
#include "object.h"
#include "string.h"
#include "tokenize.h"
#include "value.h"

#define build1ArgMathFunction(name, scrname, fun)                              \
  Value* name(Context* ctx, i64 branch, i64 line, i64 fileind) {               \
    std::vector< Value* > args = ctx->getTopFunctionArgs();                    \
    Value* result = new Value();                                               \
    if (args.size() > 0) {                                                     \
      f64 x;                                                                   \
      if (args[0]->getType() == ValueType::FLOAT) {                            \
        x = args[0]->getFloat();                                               \
      } else if (args[0]->getType() == ValueType::INT) {                       \
        x = (f64)args[0]->getInt();                                            \
      } else {                                                                 \
        if (showErrors())                                                      \
          std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)    \
                    << "' line: " << line << ": first argument for '"          \
                    << scrname << "' has to be an int or a float"              \
                    << std::endl;                                              \
        String* _s = new String("InvalidArgumentError");                       \
        result->setError(_s);                                                  \
        delete _s;                                                             \
        return result;                                                         \
      }                                                                        \
      result->setFloat(fun(x));                                                \
    } else {                                                                   \
      if (showErrors())                                                        \
        std::cout << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)     \
                  << "' line: " << line << ": '" << scrname                    \
                  << "' takes one argument";                                   \
      String* _s = new String("NotEnoughArguments");                           \
      result->setError(_s);                                                    \
      delete _s;                                                               \
    }                                                                          \
    return result;                                                             \
  }

namespace nouse {
Object* oStdMathObj;
Value* vStdMathObj;

f64 __mSign(f64 x) {
  if (x == 0.0)
    return 0.0;
  return (x > 0.0 ? 1.0 : -1.0);
}

build1ArgMathFunction(_mathSin, "sin", std::sin) build1ArgMathFunction(
    _mathCos, "cos", std::cos) build1ArgMathFunction(_mathAbs, "abs", std::abs)
    build1ArgMathFunction(_mathLog, "log", std::log)
        build1ArgMathFunction(_mathRound, "round", std::round)
            build1ArgMathFunction(_mathSqrt, "sqrt", std::sqrt)
                build1ArgMathFunction(_mathCbrt, "cbrt", std::cbrt)
                    build1ArgMathFunction(_mathSign, "sign", __mSign)

                        Value* _mathMax(
                            Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() > 0) {
    f64 x = 0.0f;
    if (args[0]->getType() == ValueType::INT) {
      x = (f64)args[0]->getInt();
    } else if (args[0]->getType() == ValueType::FLOAT) {
      x = args[0]->getFloat();
    } else {
      if (showErrors())
        std::cout
            << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
            << "' line: " << line
            << ": all arguments of 'max' have to be either int or float type";
      Value* result = new Value();
      String* _s = new String("InvalidArgument");
      result->setError(_s);
      delete _s;
    }
    for (i64 i = 1; i < args.size(); i++) {
      float y = 0.0f;
      if (args[i]->getType() == ValueType::INT) {
        y = (f64)args[i]->getInt();
      } else if (args[i]->getType() == ValueType::FLOAT) {
        y = args[i]->getFloat();
      } else {
        if (showErrors())
          std::cout
              << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
              << "' line: " << line
              << ": all arguments of 'max' have to be either int or float type";
        Value* result = new Value();
        String* _s = new String("InvalidArgument");
        result->setError(_s);
        delete _s;
      }
      if (y > x)
        x = y;
    }
    Value* result = new Value();
    result->setFloat(x);
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": 'max' takes at least one argument";
    Value* result = new Value();
    String* _s = new String("NotEnoughArguments");
    result->setError(_s);
    delete _s;
  }
}
Value* _mathMin(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() > 0) {
    f64 x = 0.0f;
    if (args[0]->getType() == ValueType::INT) {
      x = (f64)args[0]->getInt();
    } else if (args[0]->getType() == ValueType::FLOAT) {
      x = args[0]->getFloat();
    } else {
      if (showErrors())
        std::cout
            << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
            << "' line: " << line
            << ": all arguments of 'min' have to be either int or float type";
      Value* result = new Value();
      String* _s = new String("InvalidArgument");
      result->setError(_s);
      delete _s;
    }
    for (i64 i = 1; i < args.size(); i++) {
      float y = 0.0f;
      if (args[i]->getType() == ValueType::INT) {
        y = (f64)args[i]->getInt();
      } else if (args[i]->getType() == ValueType::FLOAT) {
        y = args[i]->getFloat();
      } else {
        if (showErrors())
          std::cout
              << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
              << "' line: " << line
              << ": all arguments of 'min' have to be either int or float type";
        Value* result = new Value();
        String* _s = new String("InvalidArgument");
        result->setError(_s);
        delete _s;
      }
      if (y < x)
        x = y;
    }
    Value* result = new Value();
    result->setFloat(x);
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": 'min' takes at least one argument";
    Value* result = new Value();
    String* _s = new String("NotEnoughArguments");
    result->setError(_s);
    delete _s;
  }
}

Value* _mathRandomSeed(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  i64 seed = 0;
  if (args.size() < 1)
    seed = std::time(NULL);
  else {
    if (args[0]->getType() == ValueType::INT) {
      seed = args[0]->getInt();
    } else if (args[0]->getType() == ValueType::FLOAT) {
      f64 f = args[0]->getFloat();
      seed = *(i64*)&f;
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR: file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": the argument of 'random_seed' have to be either int or "
                     "float type";
      Value* result = new Value();
      String* _s = new String("InvalidArgument");
      result->setError(_s);
      delete _s;
    }
  }

  std::srand(seed);
  // std::rand();

  Value* result = new Value();
  result->setInt(seed);
  return result;
}
Value* _mathRandom(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* result = new Value();
  result->setFloat((f64)std::rand() / (f64)RAND_MAX);
  return result;
}

NativeFunction* nfSin;
NativeFunction* nfCos;
NativeFunction* nfAbs;
NativeFunction* nfLog;
NativeFunction* nfRound;
NativeFunction* nfSqrt;
NativeFunction* nfCbrt;
NativeFunction* nfSign;
NativeFunction* nfMax;
NativeFunction* nfMin;
NativeFunction* nfRandomSeed;
NativeFunction* nfRandom;

void initStdMath() {
  oStdMathObj = new Object();
  Value* v = new Value();
  nfSin = new NativeFunction(&_mathSin);
  v->setNativeFunction(nfSin);
  oStdMathObj->setField("sin", v);
  nfCos = new NativeFunction(&_mathCos);
  v->setNativeFunction(nfCos);
  oStdMathObj->setField("cos", v);
  nfAbs = new NativeFunction(&_mathAbs);
  v->setNativeFunction(nfAbs);
  oStdMathObj->setField("abs", v);
  nfLog = new NativeFunction(&_mathLog);
  v->setNativeFunction(nfLog);
  oStdMathObj->setField("log", v);
  nfRound = new NativeFunction(&_mathRound);
  v->setNativeFunction(nfRound);
  oStdMathObj->setField("round", v);
  nfSqrt = new NativeFunction(&_mathSqrt);
  v->setNativeFunction(nfSqrt);
  oStdMathObj->setField("sqrt", v);
  nfSqrt = new NativeFunction(&_mathCbrt);
  v->setNativeFunction(nfCbrt);
  oStdMathObj->setField("cbrt", v);
  nfSqrt = new NativeFunction(&_mathSign);
  v->setNativeFunction(nfSign);
  oStdMathObj->setField("sign", v);
  nfMax = new NativeFunction(&_mathMax);
  v->setNativeFunction(nfMax);
  oStdMathObj->setField("max", v);
  nfMin = new NativeFunction(&_mathMin);
  v->setNativeFunction(nfMin);
  oStdMathObj->setField("min", v);
  nfRandomSeed = new NativeFunction(&_mathRandomSeed);
  v->setNativeFunction(nfRandomSeed);
  oStdMathObj->setField("random_seed", v);
  nfRandom = new NativeFunction(&_mathRandom);
  v->setNativeFunction(nfRandom);
  oStdMathObj->setField("random", v);

  // String* s = new String("LibMath");
  // v->setString(s);
  // delete s;
  // oStdMathObj->setField("__str", v);
  delete v;

  vStdMathObj = new Value();
  vStdMathObj->setObject(oStdMathObj);
}
void closeStdMath() {
  delete vStdMathObj;
  delete oStdMathObj;

  delete nfSin;
  delete nfCos;
  delete nfAbs;
  delete nfLog;
  delete nfRound;
  delete nfSqrt;
  delete nfCbrt;
  delete nfSign;
  delete nfMax;
  delete nfMin;
  delete nfRandomSeed;
  delete nfRandom;
}
Value* getStdMathObj() { return vStdMathObj; }
} // namespace nouse

#undef build1ArgMathFunction
