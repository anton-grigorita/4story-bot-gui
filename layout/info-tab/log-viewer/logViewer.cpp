#include "logViewer.h"

LogViewer::LogViewer() {
    fileName = "logs/execution_" + getCurrentDate() + ".log";
    autoScroll = true;
}

void LogViewer::update() {
    namespace fs = std::filesystem;
    if (!fs::exists(fileName)) return;

    auto lastWriteTime = fs::last_write_time(fileName);
    if (lastWriteTime == lastModifiedTime)
        return;

    lastModifiedTime = lastWriteTime;
    logs.clear();

    std::ifstream file(fileName, std::ios::in);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
        logs.push_back(line);
}

void LogViewer::render() {
    ImGui::BeginChild("##LogRegion", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);

    const bool wasAtBottom = autoScroll && (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f);

    for (std::string log : logs) {
        if (log.find(":: info ::") != std::string::npos)
            ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), log.c_str());

        else if (log.find(":: warning ::") != std::string::npos)
            ImGui::TextColored(ImVec4(1.0f, 0.667f, 0.133f, 1.0f), log.c_str());

        else if (log.find(":: error ::") != std::string::npos)
            ImGui::TextColored(ImVec4(0.980f, 0.153f, 0.153f, 1.0f), log.c_str());

        else if (log.find(":: debug ::") != std::string::npos)
            ImGui::TextColored(ImVec4(0.7f, 0.85f, 1.0f, 1.0f), log.c_str());

        else ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), log.c_str());
    }

    ImGui::Dummy(ImVec2(0.0f, ImGui::GetTextLineHeight() * 0.2f));

    if (wasAtBottom) ImGui::SetScrollHereY(1.0f);
 
    ImGui::EndChild();
}