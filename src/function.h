#ifndef NOUSE_FUNCTION_H
#define NOUSE_FUNCTION_H

#include <vector>

#include "classdef.h"

namespace nouse {
	class Function {
	private:
		std::vector<Value*> body;
		i64 nRefs;
		i64 ownerModule;
	public:
		Function();
		Function(std::vector<Value*>body, i64 _module=-1);
		~Function();
		Value* call(Context* ctx, i64 branch=-1);
		Function* getRef();
		void decRef();
		i64 getRefCount();
		i64 getOwnerModule();
	};
}

#endif
