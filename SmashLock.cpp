#include <ctime>
#include <iostream>
#include <Windows.h>

HHOOK hHook{ NULL };
int keysPressed = 0;
const int DELTA_LIMIT = 10;      // Time threshold in milliseconds between each key stroke
const int SEQUENTIAL_KEYS = 3;   // Sequential keys needed to trigger lock
clock_t lastMillis = clock();

LRESULT CALLBACK keyboardHook(const int code, const WPARAM wParam, const LPARAM lParam) {
	if (wParam == WM_KEYDOWN) {
		clock_t currentMillis = clock();
		if ((currentMillis - DELTA_LIMIT) >= lastMillis) {
			keysPressed = 0;
		}
		else {
			keysPressed++;
		}
		std::cout << keysPressed;
		if (keysPressed >= SEQUENTIAL_KEYS) {
			std::cout << " Smashed" << std::endl;
			LockWorkStation();
		}
		lastMillis = currentMillis;
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}

int main() {
	std::cout << "Ready to smash.." << std::endl;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);
	if (hHook == NULL) {
		std::cout << "Lemme Smash" << std::endl;
	}
	while (GetMessage(NULL, NULL, 0, 0));
	return 0;
}