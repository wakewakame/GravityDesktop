#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include "LocalHook.h"

class gdHwndManager {
private:
	static HWND desk;
	static HWND wallPaper;
	static std::vector<HWND> taskBar;
	
public:
	static HWND mine;
	static void reload();
	static inline HWND getDesk() {
		return desk;
	}
	static inline HWND getWallPaper() {
		return wallPaper;
	}
	static inline std::vector<HWND> getTaskBar() {
		return taskBar;
	}
	static void cover(HWND hwnd);
	static void hide(HWND hwnd);
	static void half(HWND hwnd);
	static void show(HWND hwnd);
	static void exit(HWND hwnd);
};