#pragma once

#include "imgui.h"
#include "common.h"
#include "logViewer.h"
#include "injector.h"
#include "spdlog/spdlog.h"

class InfoContainer {
    std::string sessionGolds;
    std::string lastSessionGolds;
    std::string goldPerHour;
    std::string avarageGoldPerHour;
    std::string petExp;
    std::string endendrosCoins;
    std::string goddesTears;

    // Variabili timer
    bool isRunning = false;
    bool isPaused = false;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pauseTime;
    std::chrono::seconds elapsedTime{ 0 };

    LogViewer logViewer;

    void startBot();
    void pauseBot();
    void resumeBot();
    void stopBot();
    std::string formatElapsedTime();

public:
    
    InfoContainer();
    void render();
};