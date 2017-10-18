#pragma once

#include "../dxtk/pch.h"
#include "Messages.h"
#include "../capture/all_inc.h"

class Mouse {
public:
	using CURSOR = DirectX::SimpleMath::Vector2; // カーソル座標
	struct CLICK {
		bool l; // 左クリック
		bool r; // 右クリック
		bool w; // ホイールクリック
		CLICK() {
			l = 0;
			r = 0;
			w = 0;
		}
	};
	using WHEEL = DirectX::SimpleMath::Vector2; // ホイール回転量

private:
	static CURSOR cursor; // 現在のマウスカーソル座標
	static CURSOR cursor_buf; // 1つ前のcursorの状態
	static CURSOR cursor_move; // 1フレーム間のカーソルの移動量
	static CURSOR drag_from; // 左、右、ホイールのいずれかがクリックされた瞬間のカーソル座標(ドラッグ用)
	static CLICK click_a; // 押されている間ずっと1のクリック情報
	static CLICK click_b; // 押されたフレームのみ1のクリック情報
	static CLICK click_c; // 離されたフレームのみ1のクリック情報
	static CLICK double_click; // ダブルクリックフラグ
	static WHEEL wheel; // 1フレーム間に発生したホイール回転量
	static bool init_flag;

	static void init();

public:
	static bool loop();

	static inline CURSOR getCursor() { return cursor; }
	static inline CURSOR getCursorMove() { return cursor_move; }
	static inline CURSOR getDragFrom() { return drag_from; }
	static inline CLICK getClickA() { return click_a; }
	static inline CLICK getClickB() { return click_b; }
	static inline CLICK getClickC() { return click_c; }
	static inline CLICK getDoubleClick() { return double_click; }
	static inline WHEEL getWheel() { return wheel; }
};