#pragma once

#include "../dxtk/pch.h"
#include "Messages.h"
#include "../capture/all_inc.h"

class Keyboard {
public:
	struct push_type {
		// âüÇ≥ÇÍÇƒÇ¢ÇÈä‘, âüÇ≥ÇÍÇΩèuä‘, ó£Ç≥ÇÍÇΩèuä‘
		bool a, b, c;
		push_type() {
			a = 0;
			b = 0;
			c = 0;
		}
	};

private:
	struct sub_push_type : public push_type {
		void set(WPARAM mes) {
			switch (mes) {
			case WM_KEYDOWN:
				a = 1;
				b = 1;
				break;
			case WM_KEYUP:
				a = 0;
				c = 1;
			}
		}
		void reset() {
			b = 0;
			c = 0;
		}
	};
	static sub_push_type Shift;
	static sub_push_type Ctrl;
	static sub_push_type Esc;

public:
	static bool loop();
	
	static inline push_type getShift() { return Shift; }
	static inline push_type getCtrl() { return Ctrl; }
	static inline push_type getEsc() { return Esc; }
};