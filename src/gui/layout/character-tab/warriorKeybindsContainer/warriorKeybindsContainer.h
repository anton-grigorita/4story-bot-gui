#pragma once

#include "imgui.h"
#include "common.h"

class WarriorKeybindsContainer {
    static ImGuiKey battleCry;
    static ImGuiKey autoAttack;
    static ImGuiKey defenceCall;

public:
    void render();
};