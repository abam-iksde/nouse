#include "value.h"

#include <stdlib.h>

#include <sstream>

#include "function.h"
#include "object.h"
#include "string.h"
#include "tuple.h"
#include "vname.h"

namespace nouse {
Value::Value() {
  this->type = ValueType::NONE;
  this->value = nullptr;
}
Value::~Value() { this->makeNone(); }
stdstr_t getValueTypeName(ValueType t) {
  switch (t) {
  case ValueType::NONE:
    return "none";
    break;
  case ValueType::NAME:
    return "name";
    break;
  case ValueType::INT:
    return "int";
    break;
  case ValueType::FLOAT:
    return "float";
    break;
  case ValueType::BOOLEAN:
    return "boolean";
    break;
  case ValueType::STRING:
    return "string";
    break;
  case ValueType::OBJECT:
    return "object";
    break;
  case ValueType::FUNCTION:
    return "function";
    break;
  case ValueType::NATIVE_FUNCTION:
    return "native_function";
    break;
  case ValueType::FUNCTION_CALL:
    return "function_call";
    break;
  case ValueType::TUPLE:
    return "tuple";
    break;
  case ValueType::_ERROR:
    return "error";
    break;
  default:
    break;
  }
  return "unknown";
}
void Value::makeNone() {
  switch (this->type) {
  case ValueType::INT:
  case ValueType::FLOAT:
  case ValueType::BOOLEAN:
    free(this->value);
    break;
  case ValueType::OBJECT: {
    Object* o = (Object*)this->value;
    o->decRef();
    // std::cout << o->getRefCount() << std::endl;
    // delete this->value;
  } break;
  case ValueType::FUNCTION: {
    Function* f = (Function*)this->value;
    f->decRef();
  } break;
  case ValueType::_ERROR:
  case ValueType::STRING:
    delete (String*)this->value;
    break;
  case ValueType::NAME:
    delete (Name*)this->value;
    break;
  case ValueType::FUNCTION_CALL:
    delete (FunctionCall*)this->value;
    break;
  case ValueType::TUPLE:
    delete (Tuple*)this->value;
    break;
  default:
    break;
  }
  this->value = nullptr;
  this->type = ValueType::NONE;
}
ValueType Value::getType() { return this->type; }
stdstr_t Value::getName() { return ((Name*)this->value)->value; }
i64 Value::getInt() { return *(i64*)this->value; }
f64 Value::getFloat() { return *(f64*)this->value; }
String* Value::getString() { return (String*)this->value; }
String* Value::getError() { return this->getString(); };
Object* Value::getObject() { return (Object*)this->value; }
Function* Value::getFunction() { return (Function*)this->value; }
NativeFunction* Value::getNativeFunction() {
  return (NativeFunction*)this->value;
}
FunctionCall* Value::getFunctionCall() { return (FunctionCall*)this->value; }
Tuple* Value::getTuple() { return (Tuple*)this->value; }

boolean Value::getBoolean() {
  switch (this->type) {
  case ValueType::BOOLEAN:
    return *(boolean*)this->value;
    break;
  case ValueType::STRING:
  case ValueType::NAME:
  case ValueType::OBJECT:
  case ValueType::FUNCTION:
  case ValueType::NATIVE_FUNCTION:
  case ValueType::FUNCTION_CALL:
  case ValueType::TUPLE:
    return b_true;
    break;
  case ValueType::INT:
    return this->getInt() != 0;
    break;
  case ValueType::FLOAT:
    return this->getFloat() != 0.0;
    break;
  }
  return b_false;
}

void* Value::getV() { return this->value; }

void Value::setName(stdstr_t v) {
  this->makeNone();
  this->value = new Name(v);
  this->type = ValueType::NAME;
}
void Value::setInt(i64 v) {
  this->makeNone();
  this->value = (i64*)malloc(sizeof(i64));
  *(i64*)this->value = v;
  this->type = ValueType::INT;
}
void Value::setFloat(f64 v) {
  this->makeNone();
  this->value = (f64*)malloc(sizeof(f64));
  *(f64*)this->value = v;
  this->type = ValueType::FLOAT;
}
void Value::setBoolean(boolean v) {
  this->makeNone();
  this->value = (boolean*)malloc(sizeof(boolean));
  *(boolean*)this->value = v;
  this->type = ValueType::BOOLEAN;
}
void Value::setString(String* v) {
  this->makeNone();
  this->value = new String(v->value);
  this->type = ValueType::STRING;
}
void Value::setObject(Object* v) {
  this->makeNone();
  this->value = (void*)v->getRef();
  this->type = ValueType::OBJECT;
}
void Value::setFunction(Function* v) {
  this->makeNone();
  this->value = (void*)v->getRef();
  this->type = ValueType::FUNCTION;
}
void Value::setNativeFunction(NativeFunction* v) {
  this->makeNone();
  this->value = (void*)v;
  this->type = ValueType::NATIVE_FUNCTION;
}
void Value::setFunctionCall(FunctionCall* v) {
  this->makeNone();
  this->value = (void*)v;
  this->type = ValueType::FUNCTION_CALL;
}
void Value::setTuple(Tuple* v, boolean keepNullptr) {
  this->makeNone();
  // this->value = (void*)v;
  std::vector< Value* > val;
  for (i64 i = 0; i < v->length(); i++) {
    if (v->data()[i] != nullptr) {
      Value* e = new Value();
      e->set(v->data()[i]);
      val.push_back(e);
    } else if (keepNullptr) {
      val.push_back(nullptr);
    }
  }
  this->value = (void*)new Tuple(val);
  this->type = ValueType::TUPLE;
}
void Value::setError(String* v) {
  this->makeNone();
  this->value = new String(v->value);
  this->type = ValueType::_ERROR;
}
void Value::makeCallMethod() {
  this->makeNone();
  this->type = ValueType::CALL_METHOD;
}
void Value::set(Value* v) {
  if (v == nullptr)
    return;
  switch (v->getType()) {
  case ValueType::NONE:
    this->makeNone();
    break;
  case ValueType::NAME: {
    Name* n = (Name*)v->value;
    this->setName(n->value);
  } break;
  case ValueType::INT:
    this->setInt(*(i64*)v->value);
    break;
  case ValueType::FLOAT:
    this->setFloat(*(f64*)v->value);
    break;
  case ValueType::BOOLEAN:
    this->setBoolean(*(boolean*)v->value);
    break;
  case ValueType::STRING:
    this->setString((String*)v->value);
    break;
  case ValueType::OBJECT:
    this->setObject((Object*)v->value);
    break;
  case ValueType::FUNCTION:
    this->setFunction((Function*)v->value);
    break;
  case ValueType::NATIVE_FUNCTION:
    this->setNativeFunction((NativeFunction*)v->value);
    break;
  case ValueType::FUNCTION_CALL: // should never happen actually
    this->setFunctionCall((FunctionCall*)v->value);
    break;
  case ValueType::TUPLE:
    this->setTuple((Tuple*)v->value);
    break;
  case ValueType::CALL_METHOD:
    this->makeCallMethod();
    break;
  case ValueType::_ERROR:
    this->setError((String*)v->value);
    break;
  default:
    this->makeNone();
    break;
  }
}

stdstr_t Value::toString(Context* ctx, i64 line, i64 fileind) {
  stdstrstream_t s;
  switch (this->getType()) {
  case ValueType::NAME: {
    Name* n = (Name*)this->value;
    s << "identifier: '" << n->value << '\'';
  } break;
  case ValueType::INT:
    s << *(i64*)this->value;
    break;
  case ValueType::FLOAT:
    s << *(f64*)this->value;
    break;
  case ValueType::BOOLEAN:
    s << (*(boolean*)this->value ? "true" : "false");
    break;
  case ValueType::_ERROR:
  case ValueType::STRING:
    s << ((String*)this->value)->value;
    break;
  case ValueType::OBJECT:
    s << ((Object*)this->value)->toString(ctx, line, fileind);
    break;
  case ValueType::FUNCTION:
    s << "@function";
    break;
  case ValueType::NATIVE_FUNCTION:
    s << "@native_function";
    break;
  case ValueType::TUPLE:
    s << ((Tuple*)this->value)->toString(ctx, line, fileind);
    break;
  case ValueType::FUNCTION_CALL: // should never happen actually
    s << "@function_call";
    break;
  default:
    s << "none";
    break;
  }
  return s.str();
}
} // namespace nouse
