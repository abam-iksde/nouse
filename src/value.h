#ifndef NOUSE_VALUE_H
#define NOUSE_VALUE_H

#include <iostream>

#include "classdef.h"

namespace nouse {
	enum class ValueType {
		NONE,
		NAME,
		INT,
		FLOAT,
		BOOLEAN,
		STRING,
		OBJECT,
		FUNCTION,
		NATIVE_FUNCTION,
		FUNCTION_CALL,
		TUPLE,
		CALL_METHOD,
		_ERROR,
	};
	stdstr_t getValueTypeName(ValueType);
	class Value {
	private:
		ValueType type;
		void* value;
	public:
		Value();
		~Value();
		void makeNone();
		ValueType getType();
		stdstr_t getName();
		i64 getInt();
		f64 getFloat();
		boolean getBoolean();
		String* getString();
		Object* getObject();
		Function* getFunction();
		NativeFunction* getNativeFunction();
		FunctionCall* getFunctionCall();
		Tuple* getTuple();
		String* getError();

		void* getV();

		void setName(stdstr_t);
		void setInt(i64);
		void setFloat(f64);
		void setBoolean(boolean);
		void setString(String*);
		void setObject(Object*);
		void setFunction(Function*);
		void setNativeFunction(NativeFunction*);
		void setFunctionCall(FunctionCall*);
		void setTuple(Tuple*,boolean keepNullptr = b_false);
		void setError(String*);
		void set(Value*);
		void makeCallMethod();

		stdstr_t toString(Context* ctx=nullptr, i64 line=-1, i64 fileind=-1);
	};
}

#endif
