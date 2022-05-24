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
}

#endif
