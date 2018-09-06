#include "hwnd_manager.h"

HWND gdHwndManager::desk;
HWND gdHwndManager::wallPaper;
std::vector<HWND> gdHwndManager::taskBar;
HWND gdHwndManager::mine;

void gdHwndManager::reload() {
	// デスクトップのハンドル取得
	{
		HWND buf_1; // ハンドルバッファ1
		HWND buf_2; // ハンドルバッファ2
		{
			// デスクトップにProgmanを使用していた場合、WorkerWに変更
			buf_1 = NULL;
			buf_2 = NULL;
			while (true) {
				buf_1 = FindWindowEx(NULL, buf_1, L"Progman", L"Program Manager");
				if (buf_1 == NULL) {
					buf_2 = NULL;
					break;
				}
				buf_2 = FindWindowEx(buf_1, NULL, L"SHELLDLL_DefView", L"");
				if (buf_2 != NULL) {
					if (SendMessageTimeout(buf_1, 0x052C, 0, 0, 0x0, 7000, NULL) == NULL) desk = NULL;
					break;
				}
			}
			// WorkerWのSysListView32ハンドル取得
			buf_1 = NULL;
			buf_2 = NULL;
			while (true) {
				buf_1 = FindWindowEx(NULL, buf_1, L"WorkerW", L"");
				if (buf_1 == NULL) {
					buf_2 = NULL;
					break;
				}
				buf_2 = FindWindowEx(buf_1, NULL, L"SHELLDLL_DefView", L"");
				if (buf_2 != NULL) {
					break;
				}
			}
		}
		if (buf_2 == NULL) desk = NULL;
		// デスクトップのリストのハンドル取得
		desk = FindWindowEx(buf_2, NULL, L"SysListView32", NULL);
	}
	// 壁紙ハンドルの取得
	{
		// WorkerWのハンドル取得
		wallPaper = NULL;
		while (true) {
			wallPaper = FindWindowEx(NULL, wallPaper, L"WorkerW", L"");
			if (wallPaper == NULL) {
				break;
			}
			if (GetParent(wallPaper) == FindWindowEx(NULL, NULL, L"Progman", L"Program Manager")) break;
		}
	}
	// タスクバーハンドル列挙
	{
		taskBar.clear();
		HWND buf;
		// メインのタスクバー取得
		buf = NULL;
		while (true) {
			buf = FindWindowEx(NULL, buf, L"Shell_TrayWnd", NULL);
			if (buf == NULL) break;
			taskBar.push_back(buf);
		}
		// サブのタスクバー取得
		buf = NULL;
		while (true) {
			buf = FindWindowEx(NULL, buf, L"Shell_SecondaryTrayWnd", NULL);
			if (buf == NULL) break;
			taskBar.push_back(buf);
		}
	}
}

void gdHwndManager::cover(HWND hwnd) {
	HWND h; // ハンドル一時代入用バッファ
	LONG style; // スタイル一時保持バッファ

	// 壁紙ウィンドウの透明度を100%にする
	// 壁紙ウィンドウを有効化する
	{
		// 壁紙
		h = wallPaper;
		show(h);
		EnableWindow(h, TRUE);
		SetActiveWindow(h);
		// 壁紙の親
		h = GetParent(h);
		show(h);
		EnableWindow(h, TRUE);
		SetActiveWindow(h);
	}

	// 親ウィンドウの設定
	SetParent(hwnd, wallPaper); // 親ウィンドウを壁紙に設定

	RECT rc;
	GetWindowRect(wallPaper, &rc);
	SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0); // ウィンドウ座標を相対座標に変更

	// デスクトップの祖先ウィンドウを透明度を0%にする
	// (デスクトップの透明度は100%のままにする)
	{
		h = GetAncestor(desk, GA_ROOTOWNER);
		hide(h);
	}
	SetHook(hwnd, GetParent(wallPaper));
}

void gdHwndManager::hide(HWND hwnd) {
	LONG style; // スタイル一時保持バッファ
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
}

void gdHwndManager::half(HWND hwnd) {
	LONG style; // スタイル一時保持バッファ
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 128, LWA_ALPHA);
}

void gdHwndManager::show(HWND hwnd) {
	LONG style; // スタイル一時保持バッファ
	style = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
}

void gdHwndManager::exit(HWND hwnd) {
	HWND h; // ハンドル一時代入用バッファ
	LONG style; // スタイル一時保持バッファ

	ResetHook();

	// デスクトップの祖先ウィンドウを透明度を0%にする
	// (デスクトップの透明度は100%のままにする)
	{
		h = GetAncestor(desk, GA_ROOTOWNER);
		show(h);
	}
}