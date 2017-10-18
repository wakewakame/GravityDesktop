#include <cstdio>
#include <tchar.h>
#include <string>
#include <sstream>
#include "LocalHook.h"

#pragma data_seg(".shareddata")
HHOOK hKeyHook = 0;
HHOOK hMouseHook = 0;
HWND hwnd = NULL;
HWND target = NULL;
DWORD tid = NULL;
RECT targetRect;
#pragma data_seg()

HINSTANCE hInst;

bool SetHook(HWND set_hwnd, HWND set_target)
{
	hwnd = set_hwnd;
	target = set_target;
	GetWindowRect(target, &targetRect);
	// ターゲットのウィンドウハンドルからスレッドID取得
	tid = GetWindowThreadProcessId(target, NULL);
	// ターゲットのウィンドウハンドルにキーフック追加
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, hInst, tid);
	// ターゲットのウィンドウハンドルにマウスフック追加
	hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseHookProc, hInst, tid);
	// エラーチェック
	if ((hKeyHook != NULL) && (hMouseHook != NULL)) return 1;
	return 0;
}

bool ResetHook()
{
	if (UnhookWindowsHookEx(hKeyHook) == 0) return 1;
	return 0;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode < 0)    //決まり事
		return CallNextHookEx(hKeyHook, nCode, wp, lp);
	if ((nCode == HC_ACTION) && (hwnd != NULL)) {
		//目的のウインドウにキーボードメッセージと、キーコードの転送

		// どこで押してもHOOKする
		//ボタンが押された状態の時限定
		PostMessage(
			hwnd,
			(lp & 0b10000000000000000000000000000000) ? WM_KEYUP : WM_KEYDOWN,
			wp, 
			lp
		);
		return 0;
	}

	return CallNextHookEx(hKeyHook, nCode, wp, lp);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wp, LPARAM lp)
{
	MOUSEHOOKSTRUCT *msg;
	if (nCode < 0)    //決まり事
		return CallNextHookEx(hKeyHook, nCode, wp, lp);
	if ((nCode == HC_ACTION) && (hwnd != NULL)) {
		//目的のウインドウにキーボードメッセージと、キーコードの転送
		msg = (MOUSEHOOKSTRUCT*)lp;
		// どこで押してもHOOKする
		//ボタンが押された状態の時限定
		ScreenToClient(target, &msg->pt);
		PostMessage(
			hwnd,
			(UINT)wp,
			(WPARAM)msg->wHitTestCode,
			MAKELPARAM(msg->pt.x, msg->pt.y)
		);
		return 0;
	}

	return CallNextHookEx(hKeyHook, nCode, wp, lp);
}

// エントリポイント
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// アタッチ
		hInst = hModule;
		//bSetHook = FALSE;
		break;
	case DLL_PROCESS_DETACH:
		// デタッチ
		break;
	}
	return TRUE;
}