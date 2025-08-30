#include "infoContainer.h"

InfoContainer::InfoContainer() {
    sessionGolds = loadConfiguration("performace", "sessionGolds", "0");
    lastSessionGolds = loadConfiguration("performace", "lastSessionGolds", "0");
    goldPerHour = loadConfiguration("performace", "goldPerHour", "0");
    avarageGoldPerHour = loadConfiguration("performace", "avarageGoldPerHour", "0");
    petExp = loadConfiguration("performace", "petExp", "0");
    endendrosCoins = loadConfiguration("performace", "endendrosCoins", "0");
    goddesTears = loadConfiguration("performace", "goddesTears", "0");
}

void InfoContainer::startBot() {
    spdlog::info("bot start");

    // inject dll
    Injector injector("TClient.exe", "p4story.dll");
    
    if (injector.inject()) {
        spdlog::info("dll succesfuly injected");

        // start timer
        isRunning = true;
        isPaused = false;
        elapsedTime = std::chrono::seconds(0);
        startTime = std::chrono::steady_clock::now();

    }
    else spdlog::error("error on dll injection");

    // call method run
}

void InfoContainer::pauseBot() {
    if (isRunning && !isPaused) {
        isPaused = true;
        pauseTime = std::chrono::steady_clock::now();
        elapsedTime += std::chrono::duration_cast<std::chrono::seconds>(pauseTime - startTime);

        spdlog::warn("bot pause");
    }
}

void InfoContainer::resumeBot() {
    if (isRunning && isPaused) {
        isPaused = false;
        startTime = std::chrono::steady_clock::now();

        spdlog::warn("bot resume");
    }
}

void InfoContainer::stopBot() {
    isRunning = false;
    isPaused = false;
    elapsedTime = std::chrono::seconds(0);

    // eject dll
    Injector injector("TClient.exe", "p4story.dll");

    if (injector.eject()) spdlog::info("dll succesfuly ejected");
    else spdlog::error("error on dll ejection");
}

std::string InfoContainer::formatElapsedTime() {
    std::chrono::seconds totalElapsed = elapsedTime;

    if (isRunning && !isPaused) {
        totalElapsed += std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startTime
        );
    }

    const auto total = totalElapsed.count();

    int hours = static_cast<int>(total / 3600);
    int minutes = static_cast<int>((total % 3600) / 60);
    int seconds = static_cast<int>(total % 60);

    std::ostringstream oss;
    oss << hours << "h "
        << std::setw(2) << std::setfill('0') << minutes << "m "
        << std::setw(2) << std::setfill('0') << seconds << "s";

    return oss.str();
}


void InfoContainer::render() {
    if (!isRunning) {
        if (ImGui::Button("Start")) startBot();
    }
    else {
        ImGui::BeginDisabled();
        ImGui::Button("Start");
        ImGui::EndDisabled();
    }

    ImGui::SameLine();


    if (!isRunning) {
        ImGui::BeginDisabled();
        ImGui::Button("Pause");
        ImGui::EndDisabled();
    }
    else {
        if (!isPaused) {
            if (ImGui::Button("Pause")) pauseBot();
        }
        else {
            if (ImGui::Button("Resume")) resumeBot();
        }
    }
    
    ImGui::SameLine();

    if (!isRunning) {
        ImGui::BeginDisabled();
        ImGui::Button("Stop");
        ImGui::EndDisabled();
    }
    else {
        if (ImGui::Button("Stop")) stopBot();
    }

    if (ImGui::BeginTable("InfoTable", 4, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 110.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Label2", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Value2", ImGuiTableColumnFlags_WidthStretch);

        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("tof:");
        ImGui::TableNextColumn(); ImGui::Text(formatElapsedTime().c_str());
        ImGui::TableNextColumn(); ImGui::Text("");
        ImGui::TableNextColumn(); ImGui::Text("");

        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("session golds:");
        ImGui::TableNextColumn(); ImGui::Text(sessionGolds.c_str());
        ImGui::TableNextColumn(); ImGui::Text("last session golds:");
        ImGui::TableNextColumn(); ImGui::Text(lastSessionGolds.c_str());

        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("gold / hour:");
        ImGui::TableNextColumn(); ImGui::Text(goldPerHour.c_str());
        ImGui::TableNextColumn(); ImGui::Text("average gold / hour:");
        ImGui::TableNextColumn(); ImGui::Text(avarageGoldPerHour.c_str());


        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("pet exp:");
        ImGui::TableNextColumn(); ImGui::Text(petExp.c_str());
        ImGui::TableNextColumn(); ImGui::Text("endendros coins:");
        ImGui::TableNextColumn(); ImGui::Text(endendrosCoins.c_str());

        ImGui::TableNextRow();
        ImGui::TableNextColumn(); ImGui::Text("goddes tears:");
        ImGui::TableNextColumn(); ImGui::Text(goddesTears.c_str());
        ImGui::TableNextColumn(); ImGui::Text("");

        ImGui::EndTable();
    }

    ImGui::SeparatorText("Log activity");

    logViewer.update();
    logViewer.render();
}