#include "Debug.h"

void Debug::cout(std::wstring text) {
	if (text == L"") return;
	text = L"\nDebug:" + text + L"";
	OutputDebugString(text.c_str());
}
void Debug::cout(wchar_t *text) {
	cout(std::wstring(text));
}
void Debug::cout(long long text) {
	cout(std::to_wstring(text));
}
void Debug::cout(double text) {
	cout(std::to_wstring(text));
}
void Debug::cout(void* text) {
	cout(std::to_wstring((unsigned long)text));
}