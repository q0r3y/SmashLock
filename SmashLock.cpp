#include <ctime>
#include <iostream>
#include <Windows.h>

HHOOK hHook{ NULL };
int sequentialKeys = 0;
const int DELTA_LIMIT = 10;
clock_t lastMillis = clock();

LRESULT CALLBACK keyboardHook(const int code, const WPARAM wParam, const LPARAM lParam) {
	if (wParam == WM_KEYDOWN) {
		clock_t currentMillis = clock();

		if ((currentMillis - DELTA_LIMIT) >= lastMillis) {
			sequentialKeys = 0;
		}
		else {
			sequentialKeys++;
		}

		std::cout << sequentialKeys << " ";

		if (sequentialKeys >= 3) {
			std::cout << "Locked" << std::endl;
			LockWorkStation();
		}
		lastMillis = currentMillis;
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}

int main() {
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);
	if (hHook == NULL) {
		std::cout << "Unable to hook keyboard" << std::endl;
	}
	while (GetMessage(NULL, NULL, 0, 0));
	return 0;
}