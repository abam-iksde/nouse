#include "nouse.h"

#include "_std_list.h"
#include "_std_math.h"
#include "tuple.h"

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
  // closeOperators();
  closeObject();
  // closeTuple();
  closeStdList();
  closeContext();
  closeStdMath();
}
} // namespace nouse
