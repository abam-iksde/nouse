#ifndef _NOUSE_STD_STRING_H
#define _NOUSE_STD_STRING_H

#include <vector>
#include "value.h"
#include "context.h"
#include "string.h"
#include "debug.h"
#include "tokenize.h"

namespace nouse {
	Value* _format(Context* ctx, i64 branch, i64 line, i64 fileind) {
		stdstr_t result = "";
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::STRING) {
				result = args[0]->getString()->value;
				size_t lastpos = 0;
				for (i64 i = 1; i < args.size(); i++) {
					size_t pos = result.find("%v", lastpos);
					if (pos != stdstr_t::npos) {
						result = result.replace(pos, 2, args[i]->toString());
						lastpos = pos + 1;
					}
				}
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'format' has to be a string" << std::endl;
				Value* v = new Value();
				String* _s = new String("InvalidArgument");
				v->setError(_s);
				delete _s;
				return v;
			}
		}
		else {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'format' takes at least one argument" << std::endl;
			Value* v = new Value();
			String* _s = new String("NotEnoughArguments");
			v->setError(_s);
			delete _s;
			return v;
		}

		Value* v = new Value();
		String* s = new String(result);
		v->setString(s);
		delete s;
		return v;
	}

	Value* _stringLength(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'str_length' takes 1 argument" << std::endl;
			Value* v = new Value();
			String* _s = new String("NotEnoughArguments");
			v->setError(_s);
			delete _s;
			return v;
		}
		if (args[0]->getType() != ValueType::STRING) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'str_length' has to be a string" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		Value* result = new Value();
		result->setInt(args[0]->getString()->value.length());
		return result;
	}
	Value* _stringCharAt(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 2) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'str_char_at' takes 2 arguments" << std::endl;
			Value* v = new Value();
			String* _s = new String("NotEnoughArguments");
			v->setError(_s);
			delete _s;
			return v;
		}
		if (args[0]->getType() != ValueType::STRING) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'str_char_at' has to be a string" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		if (args[1]->getType() != ValueType::INT) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": second argument of 'str_char_at' has to be an int" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		Value* arg0 = args[0];
		i64 ind = args[1]->getInt();
		if (ind < 0) ind = arg0->getString()->value.length() + ind;
		if (ind < 0 || ind >= arg0->getString()->value.length()) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": string character index out of range" << std::endl;
			Value* result = new Value();
			String* _s = new String("StrCharIndexOutOfRange");
			result->setError(_s);
			delete _s;
			return result;
		}
		String* _s = new String(std::string("")+arg0->getString()->value[ind]);
		Value* result = new Value();
		result->setString(_s);
		delete _s;
		return result;
	}
}

#endif
