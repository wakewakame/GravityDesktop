#include "Messages.h"
#include "Mouse.h"
#include "Keyboard.h"

std::vector<Messages::Message> Messages::add_list;
std::vector<Messages::Message> Messages::use_list;

void Messages::add(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	add_list.push_back(Message(hWnd, message, wParam, lParam));
}
void Messages::get() {
	use_list = add_list;
	add_list.clear();
	Mouse::loop();
	Keyboard::loop();
}