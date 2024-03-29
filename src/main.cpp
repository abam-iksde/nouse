﻿#include <fstream>
#include <iostream>
#include <string>

#include "nouse.h"
#include "tokenize.h"

#pragma comment(linker, "/STACK:32000000")

int main(int argc, char** argv) {
  bool doOver = false;

  nouse::setShowErrors(b_true);

  nouse::init();
  std::vector< nouse::Token > tokens;
  nouse::Context* ctx = new nouse::Context(argc, argv);

  nouse::Function* f;
  if (argc > 1) {
    std::string filepath = argv[1];
    f = nouse::compileFile(filepath);
  } else {
    doOver = true;
    f = nouse::compileString("print('> '); let f = compile_string(input(), "
                             "'command_line'); f@@(); println();",
        "cmdl");
  }

  ctx->pushBranch();
  do {
    f->call(ctx, 0);
  } while (doOver);
  ctx->popBranch();

  delete f;
  delete ctx;

  nouse::close();

  return 0;
}
