#include <vector>
#include <iostream>
#include <string>

#include "classdef.h"
#include "value.h"
#include "context.h"
#include "object.h"
#include "tuple.h"
#include "string.h"
#include "debug.h"
#include "tokenize.h"
#include "compile.h"

namespace nouse {
	i64 isNum(stdstr_t);

	Value* _getThis(Context* ctx, i64 branch, i64 line, i64 fileind) {
		return objectGetThis();
	}

	Value* _setShowErrors(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'set_show_errors' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		setShowErrors(args[0]->getBoolean());
		return new Value();
	}
	Value* _setShowUserErrors(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'set_show_user_errors' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		setShowUserErrors(args[0]->getBoolean());
		return new Value();
	}
	Value* _deleteObj(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'delete' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		else if (args[0]->getType() != ValueType::OBJECT) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'delete' has to be an object" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		Object* oObj = args[0]->getObject();
		if (oObj->hasField("__del")) {
			Value* destructor = oObj->getField("__del");
			if (destructor->getType() == ValueType::NATIVE_FUNCTION || destructor->getType() == ValueType::FUNCTION) {
				delete oObj->callMethod(ctx, "__del");
			}
		}
		if (oObj->hasField(";__del")) {
			Value* destructor = oObj->getField(";__del");
			if (destructor->getType() == ValueType::NATIVE_FUNCTION || destructor->getType() == ValueType::FUNCTION) {
				delete oObj->callMethod(ctx, ";__del");
			}
		}

		oObj->deleted = true;
		ctx->deleteObject(oObj);
		return new Value();
	}
	Value* _args(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > vars = ctx->getTopFunctionArgs();
		std::vector< Value* > args = ctx->getTopFunctionArgs(1);
		for (i64 i = 0; i < vars.size() && i < args.size(); i++) {
			vars[i]->set(args[i]);
		}
		Value* result = new Value();
		result->setInt(args.size());
		return result;
	}
	Value* _print(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		for (int i = 0; i < args.size(); i++) {
			std::cout << args[i]->toString(ctx, line, fileind);
		}

		return new Value();
	}
	Value* _printLine(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		for (int i = 0; i < args.size(); i++) {
			std::cout << args[i]->toString(ctx, line, fileind);
		}
		std::cout << std::endl;

		return new Value();
	}
	Value* _input(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		for (int i = 0; i < args.size(); i++) {
			std::cout << args[i]->toString(ctx, line, fileind);
		}

		Value* result = new Value();
		stdstr_t s;
		//std::cin >> s;
		std::getline(std::cin, s);
		String* _s = new String(s);
		result->setString(_s);
		delete _s;
		return result;
	}
	Value* _getType(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* result = new Value();
		if (args.size() > 0) {
			String* _s = new String(getValueTypeName(args[0]->getType()));
			result->setString(_s);
			delete _s;
		}
		return result;
	}

	Value* _newObject(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		Object* obj = new Object();
		ctx->newObject(obj);
		result->setObject(obj);
		return result;
	}
	Value* _newTuple(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* result = new Value();
		std::vector< Value* > v;
		for (i64 i = 0; i < args.size(); i++) {
			Value* _v = new Value();
			_v->set(args[i]);
			v.push_back(_v);
		}
		Tuple* t = new Tuple(v);
		result->setTuple(t);
		delete t;
		return result;
	}
	Value* _tupleGet(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* result = new Value();
		if (args.size() < 2) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": not enough arguments for 'tuple_get'" << std::endl;
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
		}
		else {
			if (args[0]->getType() == ValueType::TUPLE) {
				if (args[1]->getType() == ValueType::INT) {
					i64 ind = args[1]->getInt();
					if (ind >= 0 && ind < args[0]->getTuple()->data().size()) result->set(args[0]->getTuple()->data()[ind]);
					else {
						if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": index " << ind << " out of range for 'tuple_get'" << std::endl;
						String* _s = new String("TupleIndexOutOfRange");
						result->setError(_s);
						delete _s;
					}
				}
				else {
					if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": second argument for 'tuple_get' has to be an integer" << std::endl;
					String* _s = new String("InvalidTupleIndex");
					result->setError(_s);
					delete _s;
				}
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument for 'tuple_get' has to be a tuple" << std::endl;
				String* _s = new String("InvalidTuple");
				result->setError(_s);
				delete _s;
			}
		}
		return result;
	}
	Value* _createError(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::STRING || args[0]->getType() == ValueType::_ERROR) {
				if (showErrors() && showUserErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": user defined error '" << args[0]->getError()->value << "'" << std::endl;
				result->setError(args[0]->getError());
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument for 'error' has to be a string" << std::endl;
				String* _s = new String("InvalidArgumentError");
				result->setError(_s);
				delete _s;
			}
		}
		else {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'error' takes one argument" << std::endl;
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
		}
		return result;
	}
	Value* _toString(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			String* _s = new String(args[0]->toString(ctx, line, fileind));
			result->setString(_s);
			delete _s;
		}
		else {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'string' takes one argument" << std::endl;
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
		}
		return result;
	}
	Value* _toInt(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::STRING) {
				if (isNum(args[0]->getString()->value) > 0) {
					result->setInt((i64)std::stof(args[0]->getString()->value));
				}
			}
			else if (args[0]->getType() == ValueType::FLOAT) {
				result->setInt((i64)args[0]->getFloat());
			}
			else if (args[0]->getType() == ValueType::INT) {
				result->setInt(args[0]->getInt());
			}
			else if (args[0]->getType() == ValueType::BOOLEAN) {
				result->setInt(args[0]->getBoolean() ? 1 : 0);
			}
		}
		else {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'int' takes one argument" << std::endl;
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
		}
		return result;
	}
	Value* _toFloat(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::STRING) {
				if (isNum(args[0]->getString()->value) > 0) {
					result->setFloat(std::stof(args[0]->getString()->value));
				}
			}
			else if (args[0]->getType() == ValueType::FLOAT) {
				result->setFloat(args[0]->getFloat());
			}
			else if (args[0]->getType() == ValueType::INT) {
				result->setFloat((f64)args[0]->getInt());
			}
			else if (args[0]->getType() == ValueType::BOOLEAN) {
				result->setFloat(args[0]->getBoolean() ? 1.0 : 0.0);
			}
		}
		else {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'float' takes one argument" << std::endl;
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
		}
		return result;
	}

	Value* _extIf(Context* ctx, i64 branch, i64 line, i64 fileind) {
		branch--;
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		for (i64 i = 0; i < args.size(); i += 2) {
			if (i == args.size() - 1) {
				result->set(args[i]);
			}
			else {
				if (args[i]->getBoolean()) {
					result->set(args[i + 1]);
					break;
				}
			}
		}

		ctx->popBranch();
		ctx->branchPushScope(branch);
		if (result->getType() == ValueType::FUNCTION) {
			Value* f = result;
			result = f->getFunction()->call(ctx, branch);
			delete f;
		}
		else if (result->getType() == ValueType::NATIVE_FUNCTION) {
			Value* f = result;
			Value* t = f->getNativeFunction()->call(ctx, branch, line);
			result = new Value();
			result->set(t);
			if (f->getNativeFunction()->isResultDisposable()) delete t;
			delete f;
		}
		ctx->branchPopScope(branch);
		ctx->pushBranch();

		return result;
	}

	Value* _time(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		result->setInt(std::time(NULL));
		return result;
	}

	Value* _require(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'require' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		else if (args[0]->getType() != ValueType::STRING) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'require' has to be a string" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		stdstr_t modulePath = args[0]->getString()->value;
		if (ctx->loadedModules.find(modulePath) != ctx->loadedModules.end()) {
			Value* result = new Value();
			result->setObject(ctx->loadedModules[modulePath]);
			return result;
		}
		//Value* result = _newObject(ctx, branch, line, fileind);
		Value* result = new Value();
		Object* o = new Object();
		ctx->loadedModules[modulePath] = o;
		result->setObject(o);
		Function* f = compileFile(modulePath);
		i64 b = ctx->pushBranch();
		ctx->branchPushScope(b, o);
		f->call(ctx, b);
		ctx->popBranch();
		delete f;
		return result;
	}
	Value* _compileString(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": 'compile_string' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		else if (args[0]->getType() != ValueType::STRING) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": first argument of 'compile_string' has to be a string" << std::endl;
			Value* v = new Value();
			String* _s = new String("InvalidArgument");
			v->setError(_s);
			delete _s;
			return v;
		}
		Value* functionFileName = new Value();
		String* s;
		if (args.size() > 1) {
			s = new String(args[1]->toString());
		}
		else {
			s = new String("compile_string_result");
		}
		functionFileName->setString(s);
		delete s;
		Function* f = compileString(args[0]->getString()->value, functionFileName->getString()->value);
		delete functionFileName;
		ctx->trackFunction(f);
		Value* result = new Value();
		result->setFunction(f);
		return result;
	}
	// this serves as a means of countering nouse's janky parser/lexer
	// for safety, '$' should prefix every mathematical operation in brackets and every
	// negative value for now.
	Value* _arithm(Context* ctx, i64 branch, i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() < 1) {
			if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": '$' takes one argument" << std::endl;
			Value* result = new Value();
			String* _s = new String("NotEnoughArguments");
			result->setError(_s);
			delete _s;
			return result;
		}
		Value* result = new Value();
		result->set(args[0]);
		return result;
	}
}
