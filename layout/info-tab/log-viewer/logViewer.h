#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include "imgui.h"
#include "common.h"

class LogViewer {
    std::string fileName;
    std::vector<std::string> logs;
    std::filesystem::file_time_type lastModifiedTime{};
    bool autoScroll;

public: 
    LogViewer();

    void update();
    void render();
};