#include "pch.h"
#include <windows.h>
#include <cstdio>
#include <iostream>

static void CreateConsole()
{
    if (AllocConsole()) {
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);
        SetConsoleTitleW(L"Injected DLL Console");
    }
}

DWORD WINAPI thread(LPVOID lpParam)
{
    HMODULE hSelf = reinterpret_cast<HMODULE>(lpParam);
    CreateConsole();
    std::cout << "[DLL] Console creata. Modulo base: 0x" << hSelf << std::endl;
    std::cout << "[DLL] Premi END per scaricare la DLL." << std::endl;
    while (!(GetAsyncKeyState(VK_END) & 1)) Sleep(50);
    std::cout << "[DLL] Uscita richiesta. Scarico la DLL..." << std::endl;
    FreeConsole();
    FreeLibraryAndExitThread(hSelf, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, thread, hModule, 0, nullptr);
        break;
    }
    return TRUE;
}
