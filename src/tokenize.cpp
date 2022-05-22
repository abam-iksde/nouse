#include "tokenize.h"

#include "classdef.h"

#include <conio.h>

namespace nouse {
	std::vector< stdstr_t > files;
	stdstr_t* getSourceFileName(i64 ind) {
		static stdstr_t def = "unknown";
		if (ind >= 0 && ind < files.size()) return &files[ind];
		return &def;
	}

	boolean isWhitespace(wchar_t c) {
		return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
	}

	boolean isInt(stdstr_t s) {
		for (i64 i = 0; i < s.length(); i++) {
			if (s[i] < '0' || s[i] > '9') return b_false;
		}
		return b_true;
	}

	wchar_t specialChars[] = {
		'(',
		')',
		'{',
		'}',
		',',
//		'.',
		'=',
		'+',
		'-',
		'*',
		'/',
		';',
		'!',
		'\\',
		':',
		'>',
		'<',
		'@',
	};
	i64 nSpecialChars = 17;
	stdstr_t specialCharChains[] = {
		"==",
		//"+=",
		//"-=",
		//"*=",
		//"/=",
		">=",
		"<=",
		"!=",
		"/*",
		"*/",
		"->",
		"@@",
	};
	i64 nSpecialCharChains = 8;
	boolean isSpecialChar(wchar_t c) {
		for (i64 i = 0; i < nSpecialChars; i++) {
			if (c == specialChars[i]) return b_true;
		}
		return b_false;
	}
	stdstr_t getSpecialCharChain(stdstr_t s, i64& i) {
		if (i + 1 == s.length()) return stdstr_t("") + s[i];
		stdstr_t _pair = stdstr_t("") + s[i] + s[i + 1];
		for (i64 j = 0; j < nSpecialCharChains; j++) {
			if (_pair == specialCharChains[j]) {
				i++;
				return _pair;
			}
		}
		return stdstr_t("") + s[i];
	}

	void tokenizeString(stdstr_t str, std::vector< Token >& tokens, stdstr_t filename) {
		i64 fileind = -1;
		if (filename == "unknown") {
		}
		else {
			for (i64 i = 0; i < files.size(); i++) {
				if (filename == files[i]) {
					fileind = i;
					break;
				}
			}
			if (fileind < 0) {
				files.push_back(filename);
				fileind = files.size() - 1;
			}
		}

		i64 line = 1;
		stdstr_t buffer = "";
		boolean inString = b_false;
		wchar_t stringStart = '\0';
		boolean lastObjAccess = b_false;
		boolean inComment = b_false;
		for (i64 i = 0; i < str.length(); i++) {
			if (inString) {
				if (str[i] == stringStart) {
					tokens.push_back({ line, fileind, buffer });
					buffer = "";
					inString = b_false;
					lastObjAccess = b_false;
				}
				else if (str[i] == '\\') {
					i++;
					if (i >= str.length()) {
						// sheesh
						return;
					}
					switch (str[i])
					{
					case 'n':
						buffer += '\n';
						break;
					case 'r':
						buffer += '\r';
						break;
					case '\\':
						buffer += '\\';
						break;
					case '0':
						buffer += '\0';
						break;
					case '"':
					case '\'':
						buffer += str[i];
						break;
					default:
						break;
					}
				}
				else if (str[i] == '\n') line++;
				else {
					buffer += str[i];
				}
			}
			else if (inComment) {
				if (str[i] == '\n') line++;
				else if (str[i] == '*' && str[i + 1] == '/') {
					inComment = b_false;
					i++;
				}
			}
			else {
				if (isWhitespace(str[i])) {
					if (buffer.length() > 0) tokens.push_back({ line, fileind, buffer });
					buffer = "";
					if (str[i] == '\n') line++;
					lastObjAccess = b_false;
				}
				else if (isSpecialChar(str[i])) {
					if (buffer.length() > 0) tokens.push_back({ line, fileind, buffer });
					buffer = "";
					//tokens.push_back({ line, stdstr_t("") + str[i] });
					stdstr_t chn_ = getSpecialCharChain(str, i);
					if (chn_ == "/*") {
						inComment = b_true;
					}
					else {
						tokens.push_back({ line, fileind, chn_ });
						lastObjAccess = b_false;
					}
					//if (str[i] == '\\') lastObjAccess = b_true;
				}
				else if (str[i] == '.') {
					if (buffer.length() < 1) {
						tokens.push_back({ line, fileind, "." });
						lastObjAccess = b_true;
					}
					else {
						if (isInt(buffer)) {
							buffer += '.';
						}
						else {
							tokens.push_back({ line, fileind, buffer });
							tokens.push_back({ line, fileind, "." });
							lastObjAccess = b_true;
						}
					}
				}
				else if (str[i] == '"' || str[i] == '\'') {
					if (buffer.length() > 0) tokens.push_back({ line, fileind, buffer });
					buffer = "";
					buffer += str[i];
					stringStart = str[i];
					inString = b_true;
					lastObjAccess = b_false;
				}
				else {
					if (lastObjAccess) {
						if (str[i] < '0' || str[i] > '9') buffer = "'";
					}
					buffer += str[i];
					lastObjAccess = b_false;
				}
			}
		}
		if (inString) {
			// error
		}
		else {
			if (buffer.length() > 0) tokens.push_back({ line, fileind, buffer });
		}
		//for (int i = 0; i < tokens.size(); i++) {
		//	std::cout << tokens[i].text << '\n';
		//}
	}
}
