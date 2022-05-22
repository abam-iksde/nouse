#ifndef NOUSE_CONTEXT_H
#define NOUSE_CONTEXT_H

#include <iostream>
#include <map>
#include <vector>

#include "classdef.h"

namespace nouse {
	void initContext();
	void closeContext();
	Value* getVarCurrentScope();

	class Context {
	private:
		Object* globalVariables;
		std::vector<std::vector<Object*>> variables;
		std::vector<std::vector<boolean>> scopeDisposable;
		std::vector<Object*> objects;
		std::vector<Function*> functions;
		std::vector<NativeFunction*> nativeFunctions;

		std::vector< std::vector< Value* > > functionArguments;

		i64 scopeSweep_;
	public:
		std::map< stdstr_t, Object* > loadedModules;

		Context();
		~Context();
		Value* getVariable(stdstr_t name, i64 branch = -1);
		Value* setVariable(stdstr_t name, Value* v, i64 branch = -1, i64 scope = -1);
		Value* getVariableCurrentScope(stdstr_t name, i64 branch);
		Value* setGlobal(stdstr_t name, Value* v);
		void collectGarbage();
		i64 pushBranch();
		void popBranch();
		i64 branchPushScope(i64 branch, Object* obj=nullptr);
		void branchPopScope(i64 branch);

		void registerFunction(stdstr_t name, Value* (*f)(Context* ctx, i64 branch,i64 line, i64 fileind), boolean resultDisposable = b_true);
		void trackFunction(Function* f);

		void callFunction(stdstr_t name);

		void pushFunctionArgs(std::vector< Value* >& args);
		std::vector< Value* >& getTopFunctionArgs(i64 ind=0);
		void popFunctionArgs();
		void newObject(Object*);
		void deleteObject(Object*);
		i64 getNBranches();
	};
}

#endif
