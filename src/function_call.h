#ifndef NOUSE_FUNCTION_CALL_H
#define NOUSE_FUNCTION_CALL_H

#include <vector>

#include "classdef.h"

namespace nouse {
class FunctionCall {
public:
  Value* function;
  i64 line;
  i64 fileind;
  boolean appendBranch;
  std::vector< Value* > args;
  FunctionCall(Value* function,
      std::vector< Value* > args,
      i64 line = -1,
      i64 fileind = -1,
      boolean appendBranch = b_true);
  ~FunctionCall();
  Value* execute(Context* ctx, i64 branch);
};
} // namespace nouse

#endif
