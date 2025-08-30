#pragma once

#include "imgui.h"
#include "warriorKeybindsContainer.h"
#include "mageKeybindsContainer.h"

class CharacterContainer {
    ImGuiComboFlags flags = 0;
    static std::string combo_preview_value;
    WarriorKeybindsContainer warKeybindsContainer;
    MageKeybindsContainer mageKeybindsContainer;

public:
    CharacterContainer();
    void render();
};