#pragma once

#include <string>
#include <iostream>
#include <locale> 
#include <codecvt> 
#include <cstdio>
#include <Windows.h>


#include <io.h>
#include <Fcntl.h>

namespace Debug {
	void cout(std::wstring text);
	void cout(wchar_t *text);
	void cout(long long text);
	void cout(double text);
	void cout(void* text);
}