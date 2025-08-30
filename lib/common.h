#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "simpleini.h"
#include "imgui.h"

inline const char* fileName = "conf/bot_conf.ini";

inline void saveConfiguration(const char* section, const char* key, const char* value) {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(fileName);
    ini.SetValue(section, key, value);
    ini.SaveFile(fileName);
}

inline ImGuiKey loadKeybindConfiguration(const char* section, const char* key) {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(fileName);

    const char* keybind = ini.GetValue(section, key, "");
    std::string kb = keybind ? keybind : "";

    kb.erase(0, kb.find_first_not_of(" \t\r\n"));
    kb.erase(kb.find_last_not_of(" \t\r\n") + 1);

    for (int k = ImGuiKey_NamedKey_BEGIN; k < ImGuiKey_NamedKey_END; ++k)
        if (kb == ImGui::GetKeyName((ImGuiKey)k))
            return (ImGuiKey)k;

    return ImGuiKey_None;
}

inline std::string loadConfiguration(const char* section, const char* key, const char* defaultValue) {
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile(fileName);
    std::string value = ini.GetValue(section, key, defaultValue);

    const std::string chars_to_remove = " \t\r\n";
    value.erase(0, value.find_first_not_of(chars_to_remove));
    value.erase(value.find_last_not_of(chars_to_remove) + 1);

    return value;
}

inline bool containsKeybind(ImGuiKey* arr, ImGuiKey key) {
    for (int i = 0; i < IM_ARRAYSIZE(arr); ++i)
        if (arr[i] == key) return true;

    return false;
}

inline std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now);

    char buffer[11]; // "yyyy-mm-dd"
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);

    return std::string(buffer);
}