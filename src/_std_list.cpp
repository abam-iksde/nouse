#include "_std_list.h"

#include <vector>

#include "debug.h"

#include "value.h"
#include "context.h"
#include "object.h"
#include "native_function.h"
#include "string.h"
#include "tokenize.h"

namespace nouse {
	std::vector< std::vector< Value* >* > _lists;
	std::vector< i64 > _listsFreeSpots;

	NativeFunction* nfListDelete;
	Value* vListDelete;
	NativeFunction* nfListAt;
	Value* vListAt;
	NativeFunction* nfListGet;
	Value* vListGet;
	NativeFunction* nfListSet;
	Value* vListSet;
	NativeFunction* nfListLength;
	Value* vListLength;
	NativeFunction* nfListErase;
	Value* vListErase;
	NativeFunction* nfListAppend;
	Value* vListAppend;
	NativeFunction* nfListToString;
	Value* vListToString;
	Value* _listDelete(Context* ctx, i64 branch,i64 line, i64 fileind) {
		Value* _this = objectGetThis();
		Object* oThis = _this->getObject();
		i64 ind = oThis->getField(";listspot")->getInt();
		for (i64 i = 0; i < _lists[ind]->size(); i++) {
			delete (*_lists[ind])[i];
		}
		delete _this;
		delete _lists[ind];
		_listsFreeSpots.push_back(ind);
		return new Value();
	}
	Value* _listGet(Context* ctx, i64 branch,i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::INT) {
				Value* _this = objectGetThis();
				std::vector< Value* >* list = _lists[_this->getObject()->getField(";listspot")->getInt()];
				delete _this;
				i64 ind = args[0]->getInt();
				if (ind < 0) ind = list->size() + ind;
				if (ind < 0 || ind >= list->size()) {
					if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": list index out of range" << std::endl;
					String* _s = new String("ListIndexOutOfRange");
					result->setError(_s);
					delete _s;
					return result;
				}
				result->set((*list)[ind]);
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": trying to index a list with a non integer value" << std::endl;
				String* _s = new String("InvalidListIndex");
				result->setError(_s);
				delete _s;
			}
		}
		return result;
	}
	Value* _listSet(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* result = new Value();
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 1) {
			if (args[0]->getType() == ValueType::INT) {
				Value* _this = objectGetThis();
				std::vector< Value* >* list = _lists[_this->getObject()->getField(";listspot")->getInt()];
				delete _this;
				i64 ind = args[0]->getInt();
				if (ind < 0) ind = list->size() + ind;
				if (ind < 0 || ind >= list->size()) {
					if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": list index out of range" << std::endl;
					String* _s = new String("ListIndexOutOfRange");
					result->setError(_s);
					delete _s;
					return result;
				}
				(*list)[ind]->set(args[1]);
				result->set(args[1]);
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": trying to index a list with a non integer value" << std::endl;
				String* _s = new String("InvalidListIndex");
				result->setError(_s);
				delete _s;
			}
		}
		return result;
	}
	Value* _listLength(Context* ctx, i64 branch,i64 line, i64 fileind) {
		Value* result = new Value();
		Value* _this = objectGetThis();
		result->setInt(_lists[_this->getObject()->getField(";listspot")->getInt()]->size());
		delete _this;
		return result;
	}
	Value* _listErase(Context* ctx, i64 branch,i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		if (args.size() > 0) {
			if (args[0]->getType() == ValueType::INT) {
				Value* _this = objectGetThis();
				std::vector< Value* >* list = _lists[_this->getObject()->getField(";listspot")->getInt()];
				delete _this;
				i64 ind = args[0]->getInt();
				if (ind < 0) ind = list->size() + ind;
				if (ind < 0 || ind >= list->size()) {
					if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": list index out of range" << std::endl;
					Value* result = new Value();
					String* _s = new String("ListIndexOutOfRange");
					result->setError(_s);
					delete _s;
					return result;
				}
				delete (*list)[ind];
				list->erase(list->begin()+ind);
			}
			else {
				if (showErrors()) std::cout << "NOUSE ERROR file: '" << *getSourceFileName(fileind) << "' line: " << line << ": trying to index a list with a non integer value" << std::endl;
				Value* result = new Value();
				String* _s = new String("InvalidListIndex");
				result->setError(_s);
				delete _s;
				return result;
			}
		}
		return new Value();
	}
	Value* _listAppend(Context* ctx, i64 branch,i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* vList = objectGetThis();
		Object* oList = vList->getObject();
		std::vector< Value* >* _list = _lists[oList->getField(";listspot")->getInt()];
		for (i64 i = 0;i < args.size();i++) {
			Value* v = new Value();
			v->set(args[i]);
			_list->push_back(v);
		}
		Value* v = new Value();
		v->set(vList);
		delete vList;
		return v;
	}
	Value* _listToString(Context* ctx, i64 branch, i64 line, i64 fileind) {
		Value* v = objectGetThis();
		Object* oThis = v->getObject();
		i64 listInd = oThis->getField(";listspot")->getInt();
		stdstr_t result = "[\n";
		for (i64 i = 0; i < _lists[listInd]->size(); i++) {
			result += (*_lists[listInd])[i]->toString() + ", \n";
		}
		result += ']';
		Value* vResult = new Value();
		String* sResult = new String(result);
		vResult->setString(sResult);
		delete sResult;
		delete v;
		return vResult;
	}
	void initStdList() {
		nfListDelete = new NativeFunction(&_listDelete);
		vListDelete = new Value();
		vListDelete->setNativeFunction(nfListDelete);
		nfListGet = new NativeFunction(&_listGet);
		vListGet = new Value();
		vListGet->setNativeFunction(nfListGet);
		nfListSet = new NativeFunction(&_listSet);
		vListSet = new Value();
		vListSet->setNativeFunction(nfListSet);
		nfListLength = new NativeFunction(&_listLength);
		vListLength = new Value();
		vListLength->setNativeFunction(nfListLength);
		nfListErase = new NativeFunction(&_listErase);
		vListErase = new Value();
		vListErase->setNativeFunction(nfListErase);
		nfListAppend = new NativeFunction(&_listAppend);
		vListAppend = new Value();
		vListAppend->setNativeFunction(nfListAppend);
		nfListToString = new NativeFunction(&_listToString);
		vListToString = new Value();
		vListToString->setNativeFunction(nfListToString);
	}
	void closeStdList() {
		delete nfListDelete;
		delete vListDelete;
		delete nfListGet;
		delete vListGet;
		delete nfListLength;
		delete vListLength;
		delete nfListErase;
		delete vListErase;
		delete nfListAppend;
		delete vListAppend;
	}

	i64 __getListFirstSpot() {
		if (_listsFreeSpots.size() > 0) {
			i64 result = _listsFreeSpots[_listsFreeSpots.size() - 1];
			_listsFreeSpots.pop_back();
			return result;
		}
		_lists.push_back(NULL);
		return _lists.size() - 1;
	}
	Value* _newList(Context* ctx, i64 branch,i64 line, i64 fileind) {
		std::vector< Value* > args = ctx->getTopFunctionArgs();
		Value* result = new Value();
		Object* obj = new Object();
		ctx->newObject(obj);
		i64 spot = __getListFirstSpot();
		Value* temp = new Value();
		temp->setInt(spot);
		obj->setField(";listspot", temp);
		delete temp;
		std::vector< Value* >* list = new std::vector< Value* >();
		for (i64 i = 0; i < args.size(); i++) {
			Value* v = new Value();
			v->set(args[i]);
			list->push_back(v);
		}
		_lists[spot] = list;
		obj->setField(";__del", vListDelete);
		obj->setField("get", vListGet);
		obj->setField("set", vListSet);
		obj->setField("length", vListLength);
		obj->setField("erase", vListErase);
		obj->setField("append", vListAppend);
		obj->setField("__str", vListToString);
		result->setObject(obj);

		return result;
	}
}
