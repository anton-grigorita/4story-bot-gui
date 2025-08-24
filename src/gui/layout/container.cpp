#include "container.h"

Container::Container() {
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoSavedSettings;
    window_flags |= ImGuiWindowFlags_NoMove;
    tab_bar_flags |= ImGuiTabBarFlags_None;
}

void Container::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("Container", NULL, this->window_flags);
    
    if (ImGui::BeginTabBar("tabBar", tab_bar_flags)) {
        if (ImGui::BeginTabItem("Farm Spot")) {
            
            farmSpotTab.render();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Character")) {

            characterTab.render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Bot Info")) {

            infoTab.render();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Settings")) {
            
            settingsTab.render();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Help")) {

            helpTab.render();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
    ImGui::PopStyleVar();

}