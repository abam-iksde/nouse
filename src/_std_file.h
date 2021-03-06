#ifndef NOUSE_STD_FILE_H
#define NOUSE_STD_FILE_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "compile.h"
#include "context.h"
#include "debug.h"
#include "string.h"
#include "tokenize.h"
#include "value.h"

namespace nouse {
Value* _readFile(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() < 1) {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": 'file_read' takes one argument"
                << std::endl;
    Value* result = new Value();
    String* _s = new String("NotEnoughArguments");
    result->setError(_s);
    delete _s;
    return result;
  }
  Value* arg = args[0];
  if (arg->getType() != ValueType::STRING) {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line
                << ": first argument 'file_read' has to be a string"
                << std::endl;
    Value* result = new Value();
    String* _s = new String("InvalidArgument");
    result->setError(_s);
    delete _s;
    return result;
  }
  std::stringstream buffer;
  std::ifstream f(arg->getString()->value);
  buffer << f.rdbuf();
  f.close();
  String* s = new String(buffer.str());
  Value* result = new Value();
  result->setString(s);
  delete s;
  return result;
}
Value* _writeFile(Context* ctx, i64 branch, i64 line, i64 fileind) {
  std::vector< Value* > args = ctx->getTopFunctionArgs();
  if (args.size() < 2) {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line << ": 'file_write' takes two arguments"
                << std::endl;
    Value* result = new Value();
    String* _s = new String("NotEnoughArguments");
    result->setError(_s);
    delete _s;
    return result;
  }
  Value* arg1 = args[0];
  if (arg1->getType() != ValueType::STRING) {
    if (showErrors())
      std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind)
                << "' line: " << line
                << ": first argument 'file_write' has to be a string"
                << std::endl;
    Value* result = new Value();
    String* _s = new String("InvalidArgument");
    result->setError(_s);
    delete _s;
    return result;
  }
  Value* arg2 = args[1];

  std::ofstream f(arg1->getString()->value);
  f << arg2->toString();
  f.close();
  return new Value();
}
} // namespace nouse

#endif
