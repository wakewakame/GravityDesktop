#pragma once

#include <Windows.h>
#include "box.h"

namespace gdGeneral {
	const double pi = std::acos(-1.0);
	const double pi2 = 2.0 * pi;
	inline gdBox RECT2gdBox(RECT set_rect) {
		return gdBox(gdVec4(
			(float)(set_rect.left),
			(float)(set_rect.top),
			(float)(set_rect.right),
			(float)(set_rect.bottom)
		));
	}
	inline RECT gdBox2RECT(gdBox set_box) {
		RECT ret;
		ret.left = (LONG)(set_box.rect().x);
		ret.top = (LONG)(set_box.rect().y);
		ret.right = (LONG)(set_box.rect().z);
		ret.bottom = (LONG)(set_box.rect().w);
		return ret;
	}
}