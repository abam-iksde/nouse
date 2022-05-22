#include "native_function.h"

#include "value.h"
#include "context.h"

namespace nouse {
	NativeFunction::NativeFunction(Value* (*f)(Context* ctx,i64 branch,i64 line, i64 fileind), boolean resultDisposable_) {
		this->cFun = f;
		this->resultDisposable = resultDisposable_;
	}
	boolean NativeFunction::isResultDisposable() {
		return this->resultDisposable;
	}
	Value* NativeFunction::call(Context* ctx,i64 branch,i64 line, i64 fileind) {
		return this->cFun(ctx,branch,line, fileind);
	}
}
