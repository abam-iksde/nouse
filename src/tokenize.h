#ifndef NOUSE_TOKENIZE_H
#define NOUSE_TOKENIZE_H

#include <iostream>
#include <vector>

#include "classdef.h"

namespace nouse {
stdstr_t* getSourceFileName(i64 ind);

struct Token {
  i64 line;
  i64 file;
  stdstr_t text;
};
void tokenizeString(
    stdstr_t str, std::vector< Token >& tokens, stdstr_t filename = "unknown");
} // namespace nouse

#endif
