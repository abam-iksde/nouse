#include "debug.h"

namespace nouse {
boolean __showErrors = b_true;
boolean __showUserErrors = b_true;
boolean showErrors() { return __showErrors; }
boolean showUserErrors() { return __showUserErrors; }
void setShowErrors(boolean _show) { __showErrors = _show; }
void setShowUserErrors(boolean _show) { __showUserErrors = _show; }
} // namespace nouse
