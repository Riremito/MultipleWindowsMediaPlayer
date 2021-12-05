#include"../Lib/Hook.h"
#pragma comment(lib, "../Lib/Hook.lib")

HANDLE(WINAPI *_CreateMutexW)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR) = NULL;
HANDLE WINAPI CreateMutexW_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCWSTR lpName) {
	if (lpName) {
		if (wcscmp(lpName, L"Local\\Microsoft_WMP_70_CheckForOtherInstanceMutex") == 0) {
			HANDLE hRet = _CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
			HANDLE hDuplicatedMutex = NULL;
			if (DuplicateHandle(GetCurrentProcess(), hRet, 0, &hDuplicatedMutex, 0, FALSE, DUPLICATE_CLOSE_SOURCE)) {
				CloseHandle(hDuplicatedMutex);
			}
			return hRet;
		}
	}
	return _CreateMutexW(lpMutexAttributes, bInitialOwner, lpName);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hinstDLL);
		TestHook(CreateMutexW);
	}
	return TRUE;
}