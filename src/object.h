#ifndef NOUSE_OBJECT_H
#define NOUSE_OBJECT_H

#include <iostream>
#include <map>

#include "classdef.h"

namespace nouse {
	void pushObjThis(Value*);
	void popObjThis();
	void initObject();
	void closeObject();

	Value* objectGetThis();
	
	Value* _objectGet(Context* ctx, i64 branch,i64 line, i64 fileind);
	Value* _thisGet(Context* ctx, i64 branch, i64 line, i64 fileind);
	NativeFunction* thisGet();
	NativeFunction* doWithThis();
	NativeFunction* pushThis();

	//Value* functionCallGetThis();
	//Value* valueGetThis();

	class Object {
	private:
		i64 nRefs;
	public:
		std::map<stdstr_t, Value*> fields;
		Object();
		~Object();
		Value* getField(stdstr_t name);
		Value* setField(stdstr_t name, Value* v);
		Object* getRef();
		i64 getRefCount();
		boolean hasField(stdstr_t name);
		Value* callMethod(Context* ctx,stdstr_t name);
		boolean deleted;

		void decRef();

		stdstr_t toString(Context* ctx = nullptr, i64 line = -1, i64 fileind = -1);
	};
}

#endif
