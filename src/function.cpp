#include "function.h"

#include "value.h"
#include "context.h"
#include "native_function.h"
#include "function_call.h"

namespace nouse {
	Function::Function() {
		this->nRefs = 0;
	}
	Function::Function(std::vector<Value*> _body) {
		this->nRefs = 0;
		this->body = _body;
	}
	Function::~Function() {
		for (i64 i = 0; i < this->body.size(); i++) {
			delete this->body[i];
		}
	}
	Value* Function::call(Context* ctx, i64 branch) {
		Value* result = new Value();

		for (int i = 0; i < this->body.size(); i++) {
			delete result;
			i64 _branch = ctx->pushBranch();
			result = this->body[i]->getFunctionCall()->execute(ctx,_branch);
			ctx->popBranch();
		}

		return result;
	}
	Function* Function::getRef() {
		this->nRefs++;
		return this;
	}
	void Function::decRef() {
		this->nRefs--;
	}
	i64 Function::getRefCount() {
		return nRefs;
	}
}
