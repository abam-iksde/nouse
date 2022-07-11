#ifndef NOUSE_H
#define NOUSE_H

#include "classdef.h"
#include "compile.h"
#include "context.h"
#include "debug.h"
#include "function.h"
#include "object.h"
#include "operator.h"
#include "string.h"
#include "tokenize.h"
#include "value.h"
#include "vname.h"

namespace nouse {
void init();
void close();
} // namespace nouse

#endif
