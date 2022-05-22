#ifndef NOUSE_H
#define NOUSE_H

#include "debug.h"
#include "tokenize.h"
#include "classdef.h"
#include "value.h"
#include "object.h"
#include "vname.h"
#include "string.h"
#include "context.h"
#include "compile.h"
#include "function.h"
#include "operator.h"

namespace nouse {
	void init();
	void close();
}

#endif
