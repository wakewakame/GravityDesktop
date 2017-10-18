#pragma once
#include <Windows.h>
#ifdef EXPORT_
#define EXPORT_API_ __declspec(dllexport)
#else
#define EXPORT_API_ __declspec(dllimport)
#endif 

EXPORT_API_ LRESULT CALLBACK KeyHookProc(int, WPARAM, LPARAM);
EXPORT_API_ LRESULT CALLBACK MouseHookProc(int, WPARAM, LPARAM);
EXPORT_API_ bool SetHook(HWND hwnd, HWND target);
EXPORT_API_ bool ResetHook();