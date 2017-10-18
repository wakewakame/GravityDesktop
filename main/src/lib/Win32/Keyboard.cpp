#include "Keyboard.h"
#include <string>
#include "Debug.h"

Keyboard::sub_push_type Keyboard::Shift;
Keyboard::sub_push_type Keyboard::Ctrl;
Keyboard::sub_push_type Keyboard::Esc;

bool Keyboard::loop() {
	// 変数初期化
	Shift.reset();
	Ctrl.reset();
	Esc.reset();

	// メッセージ処理
	for (int i = 0; i < Messages::use_list.size(); i++) {
		LPARAM mes = Messages::use_list[i].message;
		if ((mes == WM_KEYDOWN) || (mes == WM_KEYUP)) {
			switch (Messages::use_list[i].wParam) {
			case VK_SHIFT:
				Shift.set(mes);
					break;
			case VK_CONTROL:
				Ctrl.set(mes);
				break;
			case VK_ESCAPE:
				Esc.set(mes);
				break;
			}
		}
	}

	// 処理終了
	return 0;
}