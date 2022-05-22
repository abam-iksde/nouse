#ifndef _NOUSE_STD_LIST
#define _NOUSE_STD_LIST

#include "classdef.h"

namespace nouse {
	void initStdList();
	void closeStdList();
	Value* _newList(Context* ctx, i64 branch, i64 line, i64 fileind);
}

#endif
