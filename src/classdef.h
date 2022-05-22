#ifndef NOUSE_CLASSDEF_H
#define NOUSE_CLASSDEF_H

#include <iostream>

#define b_true true
#define b_false false

namespace nouse {
	class Context;
	enum class ValueType;
	class Value;
	class Object;
	class Function;
	class NativeFunction;
	class FunctionCall;
	class Name;
	class String;
	class Tuple;
	struct Token;

	typedef bool boolean;
	typedef long long int i64;
	typedef double f64;
	typedef std::string stdstr_t;
	typedef std::stringstream stdstrstream_t;
}

#endif
