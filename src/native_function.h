#ifndef NOUSE_NATIVE_FUNCTION_H
#define NOUSE_NATIVE_FUNCTION_H

#include "classdef.h"

namespace nouse {
	class NativeFunction {
	private:
		Value* (*cFun)(Context* ctx,i64 branch,i64 line, i64 fileind);
		boolean resultDisposable;
	public:
		NativeFunction(Value* (*f)(Context* ctx,i64 branch,i64 line, i64 fileind), boolean resultDisposable = b_true);
		boolean isResultDisposable();
		Value* call(Context* ctx,i64 branch=-1,i64 line=0, i64 fileind = -1);
	};
}

#endif
