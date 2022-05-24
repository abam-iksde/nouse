#include "function.h"

#include "value.h"
#include "context.h"
#include "native_function.h"
#include "function_call.h"
#include "tokenize.h"
#include "object.h"

namespace nouse {
	Function::Function() {
		this->nRefs = 0;
		this->ownerModule = -1;
	}
	Function::Function(std::vector<Value*> _body,i64 _module) {
		this->nRefs = 0;
		this->body = _body;
		this->ownerModule = _module;
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
			if (this->ownerModule != -1) {
				Value* vModule = new Value();
				stdstr_t moduleName = *getSourceFileName(this->ownerModule);
				if (!ctx->loadedModules.count(moduleName)) {
					ctx->loadedModules[moduleName] = new Object();
				}
				vModule->setObject(ctx->loadedModules[moduleName]);
				ctx->setVariable("_", vModule, _branch);
				delete vModule;
			}
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

	i64 Function::getOwnerModule() {
		return this->ownerModule;
	}
}
