#include <Windows.h>
#include <iostream>
#include <MinHook.h>
#pragma comment(lib, "psapi.lib")
#if _WIN64
#ifdef NDEBUG
#pragma comment(lib, "libMinHook-x64-v141-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x64-v141-mt.lib")
#endif
#else
#ifdef NDEBUG
#pragma comment(lib, "libMinHook-x86-v141-mtd.lib")
#else
#pragma comment(lib, "libMinHook-x86-v141-mt.lib")
#endif
#endif

typedef int(__stdcall* MessageBoxW_t)(HWND, LPCWSTR, LPCWSTR, UINT);
MessageBoxW_t originalMessageBoxW = nullptr;
int __stdcall HookMessageBoxW(HWND hWnd, LPCWSTR lpCaption, LPCWSTR, UINT uType) {
	std::cout << "It Worked\n";
	return originalMessageBoxW(hWnd, L"Hooked", L"info", uType);
}

void StartHook() {
	std::cout << "hook starting\n";
	if (MH_Initialize() != MH_OK) {
		std::cout << "Failed to initialize minhook\n";
		return;
	}
	if (MH_CreateHook(&MessageBoxW, &HookMessageBoxW, reinterpret_cast<LPVOID*>(&originalMessageBoxW))!=MH_OK) {
		std::cout << "Failed\n";
		return;
	}
	if (MH_EnableHook(&MessageBoxW) != MH_OK) {
		std::cout << "Enable fail\n";
		return;
	}
}

int main()
{
	StartHook();
	MessageBoxW(NULL, L"Test", L"Test", MB_OK);
	MH_DisableHook(&MessageBoxW);
	return 0;
}