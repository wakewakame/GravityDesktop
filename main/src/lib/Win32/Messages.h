#pragma once

#include <Windows.h>
#include <vector>

class Messages {
public:
	struct Message {
		HWND hWnd;
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
		Message(HWND set_hWnd, UINT set_message, WPARAM set_wParam, LPARAM set_lParam) {
			hWnd = set_hWnd;
			message = set_message;
			wParam = set_wParam;
			lParam = set_lParam;
		}
	};

private:
	static std::vector<Message> add_list;

public:
	static std::vector<Message> use_list;
	static void add(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void get();
};