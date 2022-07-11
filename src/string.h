#ifndef NOUSE_STRING_H
#define NOUSE_STRING_H

#include <iostream>

#include "classdef.h"

namespace nouse {
class String {
public:
  stdstr_t value;
  String(stdstr_t value);
};
} // namespace nouse

#endif
