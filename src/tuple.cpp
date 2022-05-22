#include "tuple.h"

#include <sstream>

#include "value.h"
#include "context.h"
#include "native_function.h"

namespace nouse {
	Value* _interpretTupleLiteral(Context* ctx, i64 branch,i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* result = new Value();
		Tuple* t = new Tuple();
		for (i64 i = 0; i < args.size(); i++) {
			Value* _v = new Value();
			_v->set(args[i]);
			t->data().push_back(_v);
		}
		if (t->length() == 1) {
			result->set(t->data()[0]);
			delete t;
			return result;
		}
		result->setTuple(t);
		delete t;
		return result;
	}
	NativeFunction* nfInterpretTupleLiteral;
	NativeFunction* interpretTupleLiteral() {
		return nfInterpretTupleLiteral;
	}
	void initTuple() {
		nfInterpretTupleLiteral = new NativeFunction(&_interpretTupleLiteral);
	}

	Tuple::Tuple(std::vector< Value* >& thethings) {
		for (i64 i = 0; i < thethings.size(); i++) {
			stuff.push_back(thethings[i]);
		}
	}
	Tuple::Tuple() {}
	Tuple::~Tuple() {
		for (i64 i = 0; i < stuff.size(); i++) {
			delete stuff[i];
		}
	}
	Value* Tuple::getAt(i64 ind) {
		return stuff[ind];
	}
	i64 Tuple::length() {
		return stuff.size();
	}
	std::vector< Value* >& Tuple::data() {
		return stuff;
	}
	stdstr_t Tuple::toString(Context* ctx, i64 line, i64 fileind) {
		stdstrstream_t result;
		result << "( ";
		for (i64 i = 0; i < this->stuff.size(); i++) {
			result << this->stuff[i]->toString(ctx,line,fileind) << ", ";
		}
		result << ")";
		return result.str();
	}
}
