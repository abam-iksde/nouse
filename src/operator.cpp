#include "operator.h"

#include <map>
#include <vector>

#include "native_function.h"
#include "value.h"
#include "string.h"

#include "context.h"

#include "_operators.h"

namespace nouse {
	std::map < stdstr_t, OperatorSides > operatorsSides;
	std::vector < std::map<stdstr_t, Value*> > operators;
	void addOperator(stdstr_t s, Value* (*f)(Context* ctx, i64 branch,i64 line, i64 fileind), i64 priority, OperatorSides operatorSide, boolean resultDisposable) {
		if (priority < 0) return;
		while (operators.size() < priority + 1) {
			operators.push_back(std::map<stdstr_t, Value*>());
		}

		NativeFunction* nf = new NativeFunction(f,resultDisposable);
		Value* v = new Value();
		v->setNativeFunction(nf);

		operators[priority][s] = v;
		operatorsSides[s] = operatorSide;
	}
	boolean isOperator(Value* v) {
		if (v->getType() == ValueType::NAME) {
			std::map<stdstr_t, OperatorSides>::iterator it;
			for (it = operatorsSides.begin(); it != operatorsSides.end(); it++) {
				if (v->getName() == it->first) return b_true;
			}
		}
		return b_false;
	}
	i64 getOperatorBottomPriority() {
		return operators.size() - 1;
	}
	Value* getOperator(stdstr_t s, i64 priority) {
		if (operators[priority].find(s) == operators[priority].end()) return nullptr;
		return operators[priority][s];
	}
	OperatorSides getOperatorSides(stdstr_t s) {
		if (operatorsSides.find(s) == operatorsSides.end()) return OperatorSides::NONE;
		return operatorsSides[s];
	}

	void initOperators() {
		addOperator("==", &operator_IsEqual, 6);
		addOperator("<", &operator_IsSmaller, 6);
		addOperator("<=", &operator_IsSmallerOrEqual, 6);
		addOperator(">", &operator_IsGreater, 6);
		addOperator(">=", &operator_IsGreaterOrEqual, 6);
		addOperator("!=", &operator_IsNotEqual, 6);
		addOperator("=", &operator_Set, 11);
		addOperator("+", &operator_Add, 3);
		addOperator("-", &operator_Sub, 3);
		addOperator("*", &operator_Mult, 2);
		addOperator("%", &operator_Modulo, 2);
		addOperator("/", &operator_Div, 2);
		addOperator(".", &operator_ObjGet, 0, OperatorSides::BOTH, b_false);
		addOperator("?", &operator_If, 10);
		addOperator("->", &operator_With, 10);
		addOperator("do", &operator_Do, 10);
		addOperator("!", &operator_Not, 7, OperatorSides::RIGHT);
		addOperator("not", &operator_Not, 7, OperatorSides::RIGHT);
		addOperator("and", &operator_And, 7);
		addOperator("or", &operator_Or, 7);
		addOperator("@", &operator_DoInCurrentBranch, 1);
		addOperator("@@", &operator_DoInCurrentScope, 1);
	}
}
