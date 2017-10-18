#include "Mouse.h"
#include <string>

Mouse::CURSOR Mouse::cursor = Mouse::CURSOR(0.0, 0.0);
Mouse::CURSOR Mouse::cursor_buf = Mouse::cursor;
Mouse::CURSOR Mouse::cursor_move = Mouse::CURSOR(0.0, 0.0);
Mouse::CURSOR Mouse::drag_from = Mouse::CURSOR(0.0, 0.0);
Mouse::CLICK Mouse::click_a;
Mouse::CLICK Mouse::click_b;
Mouse::CLICK Mouse::click_c;
Mouse::CLICK Mouse::double_click;
Mouse::WHEEL Mouse::wheel = Mouse::WHEEL(0.0, 0.0);
bool Mouse::init_flag = 1;

void Mouse::init() {
	cursor.x = 0;
	cursor.y = 0;
	cursor_buf.x = 0;
	cursor_buf.y = 0;
	cursor_move.x = 0;
	cursor_move.y = 0;
	drag_from.x = 0;
	drag_from.y = 0;
	wheel.x = 0.0;
	wheel.y = 0.0;
	init_flag = 0;
}
bool Mouse::loop() {
	// 変数初期化
	if (init_flag) init();
	cursor_move = cursor - cursor_buf;
	cursor_buf = cursor;
	click_b.l = 0;
	click_b.r = 0;
	click_b.w = 0;
	click_c.l = 0;
	click_c.r = 0;
	click_c.w = 0;
	double_click.l = 0;
	double_click.r = 0;
	double_click.w = 0;
	wheel.x = 0.0;
	wheel.y = 0.0;

	// メッセージ処理
	for (int i = 0; i < Messages::use_list.size(); i++) {
		switch (Messages::use_list[i].message)
		{
		case WM_LBUTTONDBLCLK:
			double_click.l = 1;
			break;
		case WM_RBUTTONDBLCLK:
			double_click.r = 1;
			break;
		case WM_MBUTTONDBLCLK:
			double_click.w = 1;
			break;

		case WM_LBUTTONDOWN:
			click_a.l = 1;
			click_b.l = 1;
			drag_from.x = (float)((unsigned short int)(LOWORD(Messages::use_list[i].lParam)));
			drag_from.y = (float)((unsigned short int)(HIWORD(Messages::use_list[i].lParam)));
			break;
		case WM_RBUTTONDOWN:
			click_a.r = 1;
			click_b.r = 1;
			drag_from.x = (float)((unsigned short int)(LOWORD(Messages::use_list[i].lParam)));
			drag_from.y = (float)((unsigned short int)(HIWORD(Messages::use_list[i].lParam)));
			break;
		case WM_MBUTTONDOWN:
			click_a.w = 1;
			click_b.w = 1;
			drag_from.x = (float)((unsigned short int)(LOWORD(Messages::use_list[i].lParam)));
			drag_from.y = (float)((unsigned short int)(HIWORD(Messages::use_list[i].lParam)));
			break;

		case WM_LBUTTONUP:
			click_a.l = 0;
			click_c.l = 1;
			break;
		case WM_RBUTTONUP:
			click_a.r = 0;
			click_c.r = 1;
			break;
		case WM_MBUTTONUP:
			click_a.w = 0;
			click_c.w = 1;
			break;

		case WM_MOUSEMOVE:
			cursor.x = (float)((unsigned short int)(LOWORD(Messages::use_list[i].lParam)));
			cursor.y = (float)((unsigned short int)(HIWORD(Messages::use_list[i].lParam)));
			break;

		case WM_MOUSEWHEEL:
			wheel.x = 0.0f;
			wheel.y += (float)((short int)(GET_WHEEL_DELTA_WPARAM(Messages::use_list[i].lParam)));
			break;
		}
	}

	// 処理終了
	return 0;
}