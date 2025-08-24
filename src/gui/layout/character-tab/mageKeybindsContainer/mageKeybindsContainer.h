#pragma once

#include "imgui.h"
#include "common.h"

class MageKeybindsContainer {
    static ImGuiKey autoAttack;
    static ImGuiKey frostPrison;
    static ImGuiKey iceRain;
    static ImGuiKey auraOfIciness;
    static ImGuiKey captivatingWhirlwind;
    static ImGuiKey manaProtectiveBarier;
    static ImGuiKey concentration;

public:
    void render();
};