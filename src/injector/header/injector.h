#pragma once

#include <filesystem>
#include <string>
#include <cwchar>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "spdlog/spdlog.h"

class Injector {
    const char* processName;
    std::filesystem::path dllPath;

    DWORD getProcessId() const;
    HMODULE getRemoteModuleHandle(DWORD pid, const std::wstring& moduleBaseName) const;
    bool callRemoteFreeLibrary(DWORD pid, HMODULE hMod) const;

public:
    explicit Injector(const char* processName, const std::filesystem::path& dllPath);

    bool inject();     
    bool isInjected(); 
    bool eject();      
};