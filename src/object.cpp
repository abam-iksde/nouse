#include "object.h"

#include <sstream>

#include "debug.h"

#include "context.h"
#include "function.h"
#include "function_call.h"
#include "native_function.h"
#include "string.h"
#include "tokenize.h"
#include "value.h"
#include "vname.h"

namespace nouse {
std::vector< Value* > thisStack;
Value* objectGetThis() {
  Value* result = new Value();
  result->set(thisStack[thisStack.size() - 1]);
  return result;
}
// Value* vGetThis;
// Value* fvGetThis;
// NativeFunction* fGetThis;
// FunctionCall* fcGetThis;
// Value* fcvGetThis;
Object* oDefaultThis;
Value* defaultThis;

Value* _objectGet(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (args[0]->getType() == ValueType::OBJECT) {
    if (args[1]->getType() == ValueType::STRING) {
      return args[0]->getObject()->getField(args[1]->getString()->value);
    } else if (args[1]->getType() == ValueType::NAME) {
      return args[0]->getObject()->getField(args[1]->getName());
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": trying to index an object with a non-string value"
                  << std::endl;
      Value* result = new Value();
      String* _s = new String("InvalidObjectIndex");
      result->setError(_s);
      delete _s;
      return result;
    }
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": trying to index a value of type "
                << getValueTypeName(args[0]->getType()) << std::endl;
    Value* result = new Value();
    String* _s = new String("InvalidObject");
    result->setError(_s);
    delete _s;
    return result;
  }
  return new Value();
}

Value* _thisGet(Context* ctx, i64 branch, i64 line, i64 fileind) {
  Value* _this = objectGetThis();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  if (_this->getType() == ValueType::OBJECT) {
    Value* result = new Value();
    if (args[0]->getType() == ValueType::STRING) {
      result->set(_this->getObject()->getField(args[0]->getString()->value));
    } else if (args[0]->getType() == ValueType::NAME) {
      result->set(_this->getObject()->getField(args[0]->getName()));
    } else {
      if (showErrors())
        std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                  << "' line: " << line
                  << ": trying to index an object with a non-string value"
                  << std::endl;
      String* _s = new String("InvalidObjectIndex");
      result->setError(_s);
      delete _s;
      // return result;
    }
    delete _this;
    return result;
  } else {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": trying to index a value of type "
                << getValueTypeName(_this->getType()) << std::endl;
    Value* result = new Value();
    String* _s = new String("InvalidObject");
    result->setError(_s);
    delete _s;
    delete _this;
    return result;
  }
  delete _this;
  return new Value();
}

Value* _doWithThis(Context* ctx, i64 branch, i64 line, i64 fileind) {
  popObjThis();
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  Value* result = new Value();
  result->set(args[1]);
  return result;
}
Value* _pushThis(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< nouse::Value* > args = ctx->getTopFunctionArgs();
  pushObjThis(args[0]);
  return new Value();
}
NativeFunction* nfDoWithThis;
NativeFunction* nfPushThis;

NativeFunction* doWithThis() { return nfDoWithThis; }
NativeFunction* pushThis() { return nfPushThis; }

NativeFunction* nfThisGet;
NativeFunction* thisGet() { return nfThisGet; }

void initObject() {
  // vGetThis = new Value();
  // fvGetThis = new Value();
  // fGetThis = new NativeFunction(&_getThis,false);
  // fvGetThis->setNativeFunction(fGetThis);
  // fcGetThis = new FunctionCall(fvGetThis, std::vector< Value* >(), 0,
  // b_false); fcvGetThis = new Value(); fcvGetThis->setFunctionCall(fcGetThis);

  defaultThis = new Value();
  oDefaultThis = new Object();
  defaultThis->setObject(oDefaultThis->getRef());
  thisStack.push_back(defaultThis);

  nfDoWithThis = new NativeFunction(&_doWithThis);
  nfPushThis = new NativeFunction(&_pushThis);

  nfThisGet = new NativeFunction(&_thisGet);
}
void closeObject() {
  // delete fcGetThis;
  // delete fvGetThis;
  // delete fGetThis;
  // delete vGetThis;
  // delete fcvGetThis;

  delete oDefaultThis;
  delete defaultThis;

  delete nfDoWithThis;
  delete nfPushThis;

  delete nfThisGet;
}

// Value* functionCallGetThis() {
// return fcvGetThis;
//}
// Value* valueGetThis() {
//	return fvGetThis;
//}

void pushObjThis(Value* t) {
  Value* _t = new Value();
  _t->set(t);
  thisStack.push_back(_t);
}
void popObjThis() {
  delete thisStack[thisStack.size() - 1];
  thisStack.pop_back();
}

Object::Object() {
  this->nRefs = 0;
  this->deleted = false;
}
Object::~Object() {
  std::map< stdstr_t, Value* >::iterator it;
  for (it = fields.begin(); it != fields.end(); it++) {
    /*if (it->second->getType() == ValueType::OBJECT) {
            it->second->getObject()->decRef();
    }
    else if (it->second->getType() == ValueType::FUNCTION) {
            it->second->getFunction()->decRef();
    }
    else */
    delete it->second;
  }
}
boolean Object::hasField(stdstr_t name) {
  return (this->fields.find(name) != this->fields.end());
}
Value* Object::getField(stdstr_t name) {
  if (!this->hasField(name)) {
    this->fields[name] = new Value();
  }
  return this->fields[name];
}
Value* Object::setField(stdstr_t name, Value* v) {
  if (!this->hasField(name)) {
    this->fields[name] = new Value();
  }
  this->fields[name]->set(v);
  return this->fields[name];
}
Value* Object::callMethod(Context* ctx, stdstr_t name) {
  Value* result = nullptr;
  if (this->hasField(name)) {
    Value* method = this->getField(name);
    Value* _this = new Value();
    _this->setObject(this);
    pushObjThis(_this);
    i64 branch = ctx->pushBranch();
    if (method->getType() == ValueType::NATIVE_FUNCTION) {
      result = method->getNativeFunction()->call(ctx, branch, 0);
    } else if (method->getType() == ValueType::FUNCTION) {
      result = method->getFunction()->call(ctx, branch);
    }
    ctx->popBranch();
    popObjThis();
    delete _this;
  }
  return result;
}
Object* Object::getRef() {
  this->nRefs++;
  return this;
}
i64 Object::getRefCount() { return this->nRefs; }
void Object::decRef() { this->nRefs--; }

stdstr_t Object::toString(Context* ctx, i64 line, i64 fileind) {
  if (ctx != nullptr && this->hasField("__str") &&
      this->getField("__str")->getType() != ValueType::NONE) {
    Value* v = this->getField("__str");
    Value* _this = new Value();
    _this->setObject(this);
    pushObjThis(_this);
    stdstr_t s = "";
    if (v->getType() == ValueType::FUNCTION) {
      Value* result = v->getFunction()->call(ctx, ctx->pushBranch());
      ctx->popBranch();
      s = result->toString();
      delete result;
    } else if (v->getType() == ValueType::NATIVE_FUNCTION) {
      Value* result = v->getNativeFunction()->call(ctx, ctx->pushBranch());
      ctx->popBranch();
      s = result->toString();
      delete result;
    } else {
      s = v->toString();
    }
    popObjThis();
    delete _this;
    return s;
  } else {
    stdstrstream_t s;
    s << "{\n";
    std::map< stdstr_t, Value* >::iterator it;
    for (it = this->fields.begin(); it != this->fields.end(); it++) {
      s << it->first << ": " << it->second->toString() << ",\n";
    }
    s << "}";
    return s.str();
  }
}
} // namespace nouse
