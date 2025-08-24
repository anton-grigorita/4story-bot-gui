#include "Injector.h"

static HANDLE OpenTargetProcessMinimal(DWORD pid) {
    return OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, pid
    );
}

Injector::Injector(const char* processName_, const std::filesystem::path& dllPath_)
    : processName(processName_), dllPath(dllPath_) {
}

DWORD Injector::getProcessId() const {
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe{};
    pe.dwSize = sizeof(pe);

    if (Process32First(hSnap, &pe)) {
        do {
            if (!_stricmp(pe.szExeFile, processName)) {
                procId = pe.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &pe));
    }
    CloseHandle(hSnap);
    return procId;
}

HMODULE Injector::getRemoteModuleHandle(DWORD pid, const std::wstring& moduleBaseName) const {
    HMODULE result = nullptr;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snap == INVALID_HANDLE_VALUE) return nullptr;

    MODULEENTRY32W me{};
    me.dwSize = sizeof(me);
    if (Module32FirstW(snap, &me)) {
        do {
            if (_wcsicmp(me.szModule, moduleBaseName.c_str()) == 0) {
                result = me.hModule;
                break;
            }
        } while (Module32NextW(snap, &me));
    }
    CloseHandle(snap);
    return result;
}

bool Injector::isInjected() {
    const DWORD pid = getProcessId();
    if (!pid) return false;

    const std::wstring baseName = dllPath.filename().wstring();
    return getRemoteModuleHandle(pid, baseName) != nullptr;
}

bool Injector::inject() {
    const DWORD pid = getProcessId();
    spdlog::debug("pid: {}", pid);

    if (!pid) {
        spdlog::error("pid not evaluated");
        return false;
    }

    std::error_code ec;
    const auto cwd = std::filesystem::current_path(ec);
    spdlog::debug("cwd: {}", cwd.string());

    if (ec) {
        spdlog::error("impossible to evaluate working dir, ec={}", ec.value());
        return false;
    }

    const auto resolved = cwd / dllPath.filename();
    spdlog::debug("resolved path: {}", resolved.string());

    if (resolved.extension() != ".dll") {
        spdlog::error("not valid extension for dll: {}", resolved.extension().string());
        return false;
    }

    if (!std::filesystem::exists(resolved, ec) || !std::filesystem::is_regular_file(resolved, ec)) {
        spdlog::error("dll file not found into the working directory: {}", resolved.string());
        return false;
    }

    const auto absDllPath = std::filesystem::absolute(resolved, ec);
    spdlog::debug("absolute dll path: {}", absDllPath.string());

    if (ec) {
        spdlog::error("impossible to evaluate the absoulte path for the DLL");
        return false;
    }

    const std::wstring dllPathW = absDllPath.wstring();
    spdlog::debug("dll path: {}", std::filesystem::path(dllPathW).string());

    if (isInjected()) {
        spdlog::error("dll already injected");
        return true;
    }

    HANDLE hProc = OpenTargetProcessMinimal(pid);
    spdlog::debug("handle to process: {}", fmt::ptr(hProc));

    if (!hProc) {
        spdlog::error("couldn't open a handle into process, getlasterror={}", GetLastError());
        return false;
    }

    HMODULE k32Local = GetModuleHandleW(L"kernel32.dll");
    spdlog::debug("handle to kernel32.dll: {}", fmt::ptr(k32Local));

    if (!k32Local) {
        spdlog::error("getmodulehandlew(kernel32) failed, getlasterror={}", GetLastError());
        CloseHandle(hProc);
        return false;
    }

    FARPROC llwLocal = GetProcAddress(k32Local, "LoadLibraryW");
    spdlog::debug("handle to local loadlibraryw: {}", fmt::ptr(llwLocal));

    if (!llwLocal) {
        spdlog::error("getprocaddress(loadlibraryw) failed, getlasterror={}", GetLastError());
        CloseHandle(hProc);
        return false;
    }

    const ptrdiff_t llwOffset = reinterpret_cast<const BYTE*>(llwLocal) - reinterpret_cast<const BYTE*>(k32Local);
    spdlog::debug("loadlibraryw offset: {}", llwOffset);

    HMODULE k32Remote = getRemoteModuleHandle(pid, L"kernel32.dll");
    spdlog::debug("handle to remote kernel32.dll: {}", fmt::ptr(k32Remote));

    if (!k32Remote) {
        spdlog::error("impossible to get kernel32 into the remote process");
        CloseHandle(hProc);
        return false;
    }

    auto pRemoteLoadLibraryW = reinterpret_cast<LPTHREAD_START_ROUTINE>(
        reinterpret_cast<BYTE*>(k32Remote) + llwOffset
        );
    spdlog::debug("pointer to remote loadlibraryw: {}", fmt::ptr(pRemoteLoadLibraryW));

    const SIZE_T bytesToWrite = (dllPathW.size() + 1) * sizeof(wchar_t);
    spdlog::debug("bytes to write: {}", bytesToWrite);

    void* remoteBuf = VirtualAllocEx(hProc, nullptr, bytesToWrite, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    spdlog::debug("pointer to remote allocated memory: {}", fmt::ptr(remoteBuf));

    if (!remoteBuf) {
        spdlog::error("couldn't allocate memory, getlasterror={}", GetLastError());
        CloseHandle(hProc);
        return false;
    }

    SIZE_T bytesWritten = 0;
    if (!WriteProcessMemory(hProc, remoteBuf, dllPathW.c_str(), bytesToWrite, &bytesWritten) ||
        bytesWritten != bytesToWrite) {
        spdlog::error("couldn't write to process memory, getlasterror={}", GetLastError());
        VirtualFreeEx(hProc, remoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, pRemoteLoadLibraryW, remoteBuf, 0, nullptr);
    spdlog::debug("remote thread handle: {}", fmt::ptr(hThread));

    if (!hThread) {
        spdlog::error("couldn't create remote thread, getlasterror={}", GetLastError());
        VirtualFreeEx(hProc, remoteBuf, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    VirtualFreeEx(hProc, remoteBuf, 0, MEM_RELEASE);
    CloseHandle(hProc);

    const bool ok = isInjected();
    spdlog::debug("injceted: {}", ok);
    return ok;
}

bool Injector::callRemoteFreeLibrary(DWORD pid, HMODULE hMod) const {
    HANDLE hProc = OpenTargetProcessMinimal(pid);
    if (!hProc) return false;

    HANDLE hThread = CreateRemoteThread(
        hProc, nullptr, 0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(FreeLibrary),
        hMod, 0, nullptr
    );
    if (!hThread) {
        CloseHandle(hProc);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    DWORD ret = 0;
    GetExitCodeThread(hThread, &ret);

    CloseHandle(hThread);
    CloseHandle(hProc);
    return ret != 0;
}

bool Injector::eject() {
    const DWORD pid = getProcessId();
    spdlog::debug("pid: {}", pid);

    if (!pid) {
        spdlog::error("pid not evaluated");
        return false;
    }

    const std::wstring baseName = dllPath.filename().wstring();
    spdlog::debug("dll base name: {}", std::filesystem::path(baseName).string());

    constexpr int kMaxTries = 8;
    for (int i = 0; i < kMaxTries; ++i) {
        HMODULE hMod = getRemoteModuleHandle(pid, baseName);
        if (!hMod) return true;
        if (!callRemoteFreeLibrary(pid, hMod)) return false;
    }

    return getRemoteModuleHandle(pid, baseName) == nullptr;
}
