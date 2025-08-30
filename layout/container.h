#pragma once

#include "imgui.h"
#include "farmSpotContainer.h"
#include "characterContainer.h"
#include "infoContainer.h"
#include "settingsContainer.h"
#include "helpContainer.h"

class Container {
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = 0;
    ImGuiTabBarFlags tab_bar_flags = 0;

    FarmSpotContainer farmSpotTab;
    CharacterContainer characterTab;
    InfoContainer infoTab;
    SettingsContainer settingsTab;
    HelpContainer helpTab;

public:
    Container();
    void render();
};