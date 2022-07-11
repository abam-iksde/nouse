#ifndef NOUSE_VNAME_H
#define NOUSE_VNAME_H

#include <iostream>

#include "classdef.h"

namespace nouse {
class Name {
public:
  stdstr_t value;
  Name(stdstr_t value);
};
} // namespace nouse

#endif
