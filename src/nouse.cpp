#include "nouse.h"

#include "tuple.h"
#include "_std_list.h"
#include "_std_math.h"

namespace nouse {
	void init() {
		initOperators();
		initObject();
		initTuple();
		initStdList();
		initContext();
		initStdMath();
	}
	void close() {
		//closeOperators();
		closeObject();
		//closeTuple();
		closeStdList();
		closeContext();
		closeStdMath();
	}
}
