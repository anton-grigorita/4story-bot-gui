#pragma once

#include <filesystem>
#include <string>
#include <cwchar>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "spdlog/spdlog.h"

class Injector {
public:
    Injector(const char* processName_, const std::filesystem::path& dllPath_);
    bool inject();
    bool eject();                         
    bool callRemoteExport(const char* exportName, DWORD* outExitCode = nullptr) const;

private:
    DWORD getProcessId() const;
    HMODULE getRemoteModuleHandle(DWORD pid, const std::wstring& base) const;
    bool callRemoteFreeLibrary(DWORD pid, HMODULE hMod) const;
    const char* processName;
    std::filesystem::path dllPath;
};
