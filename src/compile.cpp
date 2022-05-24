#include "compile.h"

#include <string>
#include <fstream>
#include <map>

#include "function.h"
#include "tokenize.h"
#include "value.h"
#include "string.h"
#include "function_call.h"
#include "tuple.h"
#include "operator.h"
#include "native_function.h"
#include "object.h"
#include "context.h"
#include "debug.h"

namespace nouse {
	i64 isNum(stdstr_t s) {
		if (s.length() == 0) return 0;
		i64 result = 1;
		for (i64 i = 0; i < s.length(); i++) {
			if (s[i] == '.') {
				if (result == 1) result = 2;
				else return 0;
			}
			else if (s[i] < '0' || s[i] > '9') return 0;
		}
		return result;
	}

	Function* compFunction(std::vector< Token >& tokens, i64& i);

	Value* compValue(std::vector< Token >& tokens, i64& i) {
		Value* result = nullptr;
		if (tokens[i].text == "{") {
			i++;
			result = new Value();
			result->setFunction(compFunction(tokens, i));
		}
		else if (tokens[i].text[0] == '\'' || tokens[i].text[0] == '"') {
			result = new Value();
			String* _s = new String(&tokens[i].text[1]);
			result->setString(_s);
			delete _s;
		}
		else if (tokens[i].text == "true") {
			result = new Value();
			result->setBoolean(b_true);
		}
		else if (tokens[i].text == "false") {
			result = new Value();
			result->setBoolean(b_false);
		}
		/*else if (tokens[i].text == "this") {
			//delete result;
			//result = functionCallGetThis();
			result = new Value();
			result->setFunctionCall(new FunctionCall(valueGetThis(), std::vector< Value* >(),tokens[i].line,tokens[i].file));
		}*/
		else if (tokens[i].text == ",") {
			return nullptr;
		}
		else if (tokens[i].text[0] >= '0' && tokens[i].text[0] <= '9') {
			i64 type = isNum(tokens[i].text);
			if (type == 0) {
				// error
			}
			else {
				result = new Value();
				if (type == 1) { // int
					result->setInt(std::stoi(tokens[i].text));
				}
				else { // float
					result->setFloat(std::stof(tokens[i].text));
				}
			}
		}
		else if (tokens[i].text == "(") {
			i++;
			std::vector< Value* > v;
			while (tokens[i].text != ")") {
				v.push_back(compValue(tokens, i));
				i++;
			}
			result = new Value();
			Tuple* t = new Tuple(v);
			result->setTuple(t, b_true);
			delete t;
		}
		else if (tokens[i].text == ":") {
			result = new Value();
			result->makeCallMethod();
		}
		else {
			result = new Value();
			result->setName(tokens[i].text);
		}
		return result;
	}
	void scanCallMethod(std::vector< Value* >& expr,i64 line, i64 fileind) {
		for (i64 i = 0; i < expr.size(); i++) {
			if (expr[i] != nullptr)
			if (expr[i]->getType() == ValueType::CALL_METHOD) {
				if (i - 1 >= 0 && i + 1 < expr.size() && expr[i+1]->getType() == ValueType::FUNCTION_CALL) {
					Value* functionCall = nullptr;
					functionCall = expr[i + 1];
					Value* caller = expr[i - 1];
					Value* vPushThis = new Value();
					vPushThis->setNativeFunction(pushThis());
					std::vector< Value* > args;
					args.push_back(caller);
					FunctionCall* fcPushThis = new FunctionCall(vPushThis, args, line, fileind);
					Value* vfcPushThis = new Value();
					vfcPushThis->setFunctionCall(fcPushThis);

					FunctionCall* fcFunctionCall = functionCall->getFunctionCall();
					NativeFunction* nfObjectGet = thisGet();
					Value* vnfObjectGet = new Value();
					vnfObjectGet->setNativeFunction(nfObjectGet);
					std::vector< Value* > args2;
					//args2.push_back(caller);
					Value* vmethod = new Value();
					String* _s = new String(fcFunctionCall->function->getName());
					vmethod->setString(_s);
					delete _s;
					args2.push_back(vmethod);
					FunctionCall* fcObjectGetMethod = new FunctionCall(vnfObjectGet, args2, line, fileind);
					Value* vfcObjectGetMethod = new Value();
					vfcObjectGetMethod->setFunctionCall(fcObjectGetMethod);
					fcFunctionCall->function = vfcObjectGetMethod;

					Value* vDoWithThis = new Value();
					vDoWithThis->setNativeFunction(doWithThis());
					std::vector < Value* > args1;
					args1.push_back(vfcPushThis);
					args1.push_back(functionCall);
					FunctionCall* fcDoWithThis = new FunctionCall(vDoWithThis, args1, line, fileind);
					Value* vfcDoWithThis = new Value();
					vfcDoWithThis->setFunctionCall(fcDoWithThis);

					delete expr[i];
					expr[i - 1] = vfcDoWithThis;
					expr.erase(expr.begin() + i);
					expr.erase(expr.begin() + i);
					i--;
				}
				else {
					if (showErrors()) std::cout << "NOUSE COMPILATION ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": malformed method call" << std::endl;
				}
			}
		}
	}
	void scanOperator(std::vector< Value* >& expr,i64 line, i64 fileind) {
		for (i64 j = 0; j <= getOperatorBottomPriority(); j++) {
			for (i64 i = 0; i < expr.size(); i++) {
				if (expr[i] != nullptr)
				if (expr[i]->getType() == ValueType::NAME) {
					Value* op = getOperator(expr[i]->getName(), j);
					if (op != nullptr) {
						switch (getOperatorSides(expr[i]->getName())) {
						case OperatorSides::BOTH: {
							Value* f = new Value();
							std::vector< Value* > arg;
							boolean leftValid = b_false;
							boolean rightValid = b_false;
							if (i - 1 >= 0 && !isOperator(expr[i - 1])) {
								arg.push_back(expr[i - 1] != nullptr ? expr[i - 1] : new Value());
								leftValid = b_true;
							}
							else arg.push_back(new Value());
							if (i + 1 < expr.size() && !isOperator(expr[i + 1])) {
								arg.push_back(expr[i + 1] != nullptr ? expr[i + 1] : new Value());
								rightValid = b_true;
							}
							else arg.push_back(new Value());
							FunctionCall* fc = new FunctionCall(op, arg, line, fileind, b_false);
							delete expr[i];
							if (rightValid) expr.erase(expr.begin() + i + 1);
							if (leftValid) {
								expr.erase(expr.begin() + i - 1);
								i--;
							}
							f->setFunctionCall(fc);
							expr[i] = f;
						}break;
						case OperatorSides::LEFT: {
							Value* f = new Value();
							std::vector< Value* > arg;
							boolean leftValid = b_false;
							if (i - 1 >= 0 && !isOperator(expr[i - 1])) {
								arg.push_back(expr[i - 1] != nullptr ? expr[i - 1] : new Value());
								leftValid = b_true;
							}
							else arg.push_back(new Value());
							FunctionCall* fc = new FunctionCall(op, arg, line, fileind, b_false);
							delete expr[i];
							if (leftValid) {
								expr.erase(expr.begin() + i);
								i--;
							}
							f->setFunctionCall(fc);
							expr[i] = f;
						}break;
						case OperatorSides::RIGHT: {
							Value* f = new Value();
							std::vector< Value* > arg;
							boolean rightValid = b_false;
							if (i + 1 < expr.size() && !isOperator(expr[i + 1])) {
								arg.push_back(expr[i + 1] != nullptr ? expr[i + 1] : new Value());
								rightValid = b_true;
							}
							else arg.push_back(new Value());
							FunctionCall* fc = new FunctionCall(op, arg, line, fileind, b_false);
							delete expr[i];
							if (rightValid) expr.erase(expr.begin() + i + 1);
							//i--;
							f->setFunctionCall(fc);
							expr[i] = f;
						}break;
						}
					}
				}
			}
		}
	}
	void scanLocalVarDeclaration(std::vector< Value* >& expr,i64 line, i64 fileind) {
		for (i64 j = 0; j < expr.size(); j++) {
			if (expr[j] != nullptr) {
				if (expr[j]->getType() == ValueType::NAME) {
					if (j + 1 >= expr.size()) {
						// error
					}
					else if (expr[j]->getName() == "let") {
						if (expr[j + 1] != nullptr && expr[j + 1]->getType() == ValueType::NAME) {
							std::vector< Value* > args;
							Value* arg = new Value();
							String* _s = new String(expr[j + 1]->getName());
							arg->setString(_s);
							delete _s;
							args.push_back(arg);
							FunctionCall* fc = new FunctionCall(getVarCurrentScope(), args, line, fileind);
							delete expr[j];
							delete expr[j + 1];
							Value* v = new Value();
							v->setFunctionCall(fc);
							expr[j] = v;
							expr.erase(expr.begin() + j + 1);
						}
						else {
							// error
						}
					}
				}
			}
		}
	}

	void scanFunctionCall(std::vector< Value* >& expr,i64 line, i64 fileind) {
		scanLocalVarDeclaration(expr, line, fileind);
		for (i64 j = 0; j < expr.size(); j++) {
			if (expr[j] != nullptr) {
				if (expr[j]->getType() == ValueType::TUPLE) {
					//if (j > 0 && expr[j - 1] != nullptr && !isOperator(expr[j - 1])) {
						Value* f = new Value();
						std::vector< Value* > arg = expr[j]->getTuple()->data();
						scanFunctionCall(arg,line,fileind);
						Value* func;
						boolean decj;
						if (j > 0 && expr[j - 1] != nullptr && !isOperator(expr[j - 1])) {
							func = expr[j - 1];
							decj = b_true;
						}
						else {
							func = new Value();
							func->setNativeFunction(interpretTupleLiteral());
							decj = b_false;
						}
						FunctionCall* fc = new FunctionCall(func, arg, line, fileind);
						//delete expr[j];
						if (decj) {
							expr.erase(expr.begin() + j);
							j--;
						}
						f->setFunctionCall(fc);
						expr[j] = f;
					//}
					/*else {
						scanFunctionCall(expr[j]->getTuple()->data(),line);
						if (expr[j]->getTuple()->data().size() == 1) {
							Value* v = expr[j]->getTuple()->data()[0];
							delete expr[j];
							expr[j] = v;
						}
					}*/
				}
			}
		}
		for (i64 i = 0; i < expr.size(); i++) {
			if (expr[i] == nullptr) {
				expr.erase(expr.begin() + i);
				i--;
			}
		}
		scanCallMethod(expr, line, fileind);
		scanOperator(expr, line, fileind);
	}
		
	Function* compFunction(std::vector< Token >& tokens, i64& i) {
		std::vector<Value*> body;
		std::vector<Value*> expr;

		for (;i < tokens.size();i++) {
			if (tokens[i].text == "}") {
				if (expr.size() > 0) {
					scanFunctionCall(expr, tokens[i].line, tokens[i].file);
					Value* v = new Value();
					v->setNativeFunction(interpretTupleLiteral());
					FunctionCall* fcall = new FunctionCall(v, expr);
					Value* vv = new Value();
					vv->setFunctionCall(fcall);
					body.push_back(vv);
					expr.clear();
				}
				break;
			}
			else if (tokens[i].text == ";") {
				scanFunctionCall(expr, tokens[i].line, tokens[i].file);
				Value* v = new Value();
				v->setNativeFunction(interpretTupleLiteral());
				FunctionCall* fcall = new FunctionCall(v,expr);
				Value* vv = new Value();
				vv->setFunctionCall(fcall);
				body.push_back(vv);
				expr.clear();
			}
			else {
				Token* token = &tokens[i];
				expr.push_back(compValue(tokens, i));
			}
		}

		if (expr.size() > 0) {
			scanFunctionCall(expr,tokens[tokens.size()-1].line, tokens[tokens.size() - 1].file);
			Value* v = new Value();
			v->setNativeFunction(interpretTupleLiteral());
			FunctionCall* fcall = new FunctionCall(v, expr);
			Value* vv = new Value();
			vv->setFunctionCall(fcall);
			body.push_back(vv);
		}

		Function* result = new Function(body, tokens[0].file);
		return result;
	}

	Function* compileTokens(std::vector< Token >& tokens) {
		i64 i = 0;
		return compFunction(tokens, i);
	}
	Function* compileString(stdstr_t s,stdstr_t filename) {
		std::vector< Token > tokens;
		tokenizeString(s,tokens,filename);
		return compileTokens(tokens);
	}
	Function* compileFile(stdstr_t filepath) {
		stdstr_t s = "";

		std::ifstream ifile(filepath);
		stdstr_t buffer;
		while (std::getline(ifile, buffer)) {
			s += buffer + '\n';
		}
		ifile.close();

		return compileString(s,filepath);
	}
}
