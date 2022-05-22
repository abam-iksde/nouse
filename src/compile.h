#ifndef NOUSE_COMPILE_H
#define NOUSE_COMPILE_H

#include <vector>
#include <iostream>
#include "classdef.h"

namespace nouse {
	Function* compileTokens(std::vector< Token >& tokens);
	Function* compileString(stdstr_t, stdstr_t filename = "unknown");
	Function* compileFile(stdstr_t path);
	Value* compValue(std::vector< Token >& tokens, i64& i);
}

#endif
