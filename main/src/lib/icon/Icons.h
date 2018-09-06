#pragma once

#include "icon.h"
#include "../common/general.h"
#include <vector>
#include <Windows.h>
#include <Commctrl.h>

class gdIcons {
private:
	HWND hwnd = NULL; // 指定されたSysListViewのウィンドウハンドル
	HANDLE pid_h = NULL; // 指定されたSysListViewのPIDハンドル
	LPVOID ptr = nullptr; // 指定されたSysListViewに生成したメモリの先頭ポインタ
	unsigned int num_items; // アイコン総数
	LVITEM lvi; // 一時バッファ
	ULONG_PTR numRead; // 一時バッファ

public:
	std::vector<gdIcon> icon; // 指定されたSysListView内に存在するアイコン
	bool init(HWND set_hwnd){
		// ハンドル代入
		hwnd = set_hwnd;
		// PID取得
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		if (pid == NULL) return 1;
		// PIDハンドル取得
		pid_h = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE, false, pid);
		if (pid_h == NULL) return 1;
		// PIDにメモリ確保
		ptr = VirtualAllocEx(pid_h, NULL, sizeof(RECT) + sizeof(LVITEM), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ptr == NULL) return 1;
		// 更新
		update();
		// 処理終了
		return 0;
	}
	bool update() {
		// デスクトップのアイコン数取得
		num_items = ListView_GetItemCount(hwnd);
		// アイコン数に変更があれば配列長更新
		if (icon.size() != num_items) {
			icon = std::vector<gdIcon>(num_items);
			for (size_t i = 0; i < icon.size(); i++) {
				icon[i].init(hwnd, pid_h, ptr, i);
			}
		}
		// 全アイコン情報取得
		for (size_t i = 0; i < icon.size(); i++) {
			if(icon[i].update()) return 1;
		}
		return 0;
	}
	bool unselect() {
		// アイテムのステータス変更
		lvi.state = 0;
		lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED;
		if (WriteProcessMemory(pid_h, ptr, &lvi, sizeof(LVITEM), &numRead) == 0) return 1; // ptrにitem代入
		SendMessage(hwnd, LVM_SETITEMSTATE, -1, (LPARAM)ptr);
		// フォーカスを有効化する為にウィンドウをアクティブにする
		SendMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
		return 0;
	}
	bool unhot() {
		// 偽のマウス座標を送信してホットアイテムを解除
		PostMessage(hwnd, WM_MOUSEMOVE, 0, MAKELPARAM(0, 0));
		return 0;
	}
	inline HWND getHwnd() { return hwnd; }
};