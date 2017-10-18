#pragma once

#include "../common/box.h"
#include "../common/general.h"

class gdIcon {
private:
	HWND hwnd = NULL;
	HANDLE pid_h = NULL; // 指定されたSysListViewのPIDハンドル
	LPVOID ptr = nullptr; // 指定されたSysListViewに生成したメモリの先頭ポインタ
	RECT rc; // 一時バッファ
	LVITEM lvi; // 一時バッファ
	ULONG_PTR numRead; // 一時バッファ
	unsigned int index = -1;
	bool select_var = 0;
	bool focus_var = 0;
	bool hot_var = 0;

public:
	gdBox ico;
	gdBox all;
	void init(HWND set_hwnd, HANDLE set_pid_h, LPVOID set_ptr, unsigned int set_index) {
		hwnd = set_hwnd;
		pid_h = set_pid_h;
		ptr = set_ptr;
		index = set_index;
	}
	bool update() {
		// アイテムのアイコン範囲座標取得
		rc.left = LVIR_ICON; // アイコン部分とテキスト部分の両方の範囲取得
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptrにRECT代入
		SendMessage(hwnd, LVM_GETITEMRECT, index, (LPARAM)ptr); // 取得要求と先頭ポインタ送信
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // 範囲座標取得
		ico = gdGeneral::RECT2gdBox(rc);
		// アイテムの全範囲座標取得
		rc.left = LVIR_BOUNDS; // アイコン部分とテキスト部分の両方の範囲取得
		if (WriteProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // ptrにitem代入
		SendMessage(hwnd, LVM_GETITEMRECT, index, (LPARAM)ptr); // 取得要求と先頭ポインタ送信
		if (ReadProcessMemory(pid_h, ptr, &rc, sizeof(RECT), &numRead) == 0) return 1; // 範囲座標取得
		all = gdGeneral::RECT2gdBox(rc);
		// 選択、フォーカス情報取得
		UINT ret = SendMessage(hwnd, LVM_GETITEMSTATE, index, LVIS_SELECTED | LVIS_FOCUSED);
		select_var = ((ret&LVIS_SELECTED) == LVIS_SELECTED);
		focus_var = ((ret&LVIS_FOCUSED) == LVIS_FOCUSED);
		// ホットアイテム情報取得
		hot_var = (index == SendMessage(hwnd, LVM_GETHOTITEM, 0, 0));
		// 処理終了
		return 0;
	}
	bool select(UINT state, UINT stateMast) {
		// アイテムのステータス変更
		lvi.state = state;
		lvi.stateMask = stateMast;
		if (WriteProcessMemory(pid_h, ptr, &lvi, sizeof(LVITEM), &numRead) == 0) return 1; // ptrにitem代入
		SendMessage(hwnd, LVM_SETITEMSTATE, index, (LPARAM)ptr);
		// フォーカスを有効化する為にウィンドウをアクティブにする
		SendMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
		// 代入
		select_var = 1;
		focus_var = 1;
		// 処理終了
		return 0;
	}
	bool hot() {
		// ホットアイテム設定
		SendMessage(hwnd, LVM_SETHOTITEM, index, 0);
		// 代入
		hot_var = 1;
		// 処理終了
		return 0;
	}
	inline bool getSelect() { return select_var; }
	inline bool getFocus() { return focus_var; }
	inline bool getHot() { return hot_var; }
};