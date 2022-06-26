#ifndef NOUSE_STD_FILE_H
#define NOUSE_STD_FILE_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "value.h"
#include "context.h"
#include "compile.h"
#include "tokenize.h"
#include "debug.h"
#include "string.h"

namespace nouse {
	Value* _readFile(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector<Value*> args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'file_read' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		Value* arg = args[0];
		if (arg->getType() != ValueType::STRING) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument 'file_read' has to be a string" << std::endl;
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
}

#endif
