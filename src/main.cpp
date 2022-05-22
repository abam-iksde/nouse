#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <cstdlib>

#include <Windows.h>

#include "nouse.h"

#pragma comment(linker, "/STACK:32000000")

std::wstring charptrToWstring(char* s) {
	std::wstring ws(&s[0], &s[std::strlen(s)]);
	return ws;
}

//#define DEV_MODE

int main(int argc,char** argv) {
	//SetConsoleOutputCP(CP_UTF8);
	//setvbuf(stdout, nullptr, _IOFBF, 1000);
	
	bool doOver = false;

	nouse::setShowErrors(b_true);
	
	nouse::init();
	std::vector< nouse::Token > tokens;
	nouse::Context* ctx = new nouse::Context();

	nouse::Function* f;
	if (argc > 1) {
		std::string filepath = argv[1];
		f = nouse::compileFile(filepath);
	}
	else {
		doOver = true;
		f = nouse::compileString("print('> '); let f = function_from_string(input(), 'command_line'); f@@(); println();");
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
