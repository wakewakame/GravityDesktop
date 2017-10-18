#pragma once

#include "..\DXTK\pch.h"
#include "..\capture\all_inc.h"
#include "..\Win32\all_inc.h"
#include "..\imageIcon\imageIcons.h"
#include "..\common\general.h"

class Anime {
private:
	HWND hwnd;
	gdImageIcons icons;
	bool exit_step = 0;

public:

	bool init(ID3D11DeviceContext1* context, HWND set_hwnd) {
		hwnd = set_hwnd;
		// Box2D‰Šú‰»
		if (icons.init(hwnd, context)) return 1;
		return 0;
	}

	void setWallObj(HWND set_hwnd) {
		icons.setWallObj(set_hwnd);
	}

	bool render() {
		// •`‰æ
		if (exit_step == 1) PostMessage(gdHwndManager::mine, WM_CLOSE, NULL, NULL);
		if (icons.update()) exit_step = 1;
		if (exit_step == 0) icons.render();

		// ˆ—‚ÌI—¹
		return 0;
	}
};