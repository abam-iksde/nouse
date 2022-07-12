#ifndef NOUSE_OPERATOR_H
#define NOUSE_OPERATOR_H

#include "classdef.h"
#include <iostream>

namespace nouse {
enum class OperatorSides : i64 {
  NONE = 0,
  LEFT = 1,
  RIGHT = 2,
  BOTH = 3,
};

void addOperator(stdstr_t s,
    Value* (*f)(Context* ctx, i64 branch, i64 line, i64 fileind),
    i64 priority,
    OperatorSides operatorSide = OperatorSides::BOTH,
    boolean resultDisposable = b_true);
i64 getOperatorBottomPriority();
// i64 getOperatorTopPriority();
Value* getOperator(stdstr_t s, i64 priority);
void initOperators();
OperatorSides getOperatorSides(stdstr_t s);
boolean isOperator(Value* v);
} // namespace nouse

#endif
