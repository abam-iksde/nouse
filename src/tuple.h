#ifndef NOUSE_TUPLE_H
#define NOUSE_TUPLE_H

#include <iostream>
#include <vector>

#include "classdef.h"

namespace nouse {
void initTuple();
NativeFunction* interpretTupleLiteral();

class Tuple {
private:
  std::vector< Value* > stuff;

public:
  i64 length();
  Value* getAt(i64 ind);
  Tuple(std::vector< Value* >&);
  Tuple();
  ~Tuple();
  std::vector< Value* >& data();
  stdstr_t toString(Context* ctx = nullptr, i64 line = -1, i64 fileind = -1);
};
} // namespace nouse

#endif
