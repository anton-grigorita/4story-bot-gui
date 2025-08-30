#pragma once

#include "imgui.h"
#include "common.h"

class FarmSpotContainer {
    static std::string combo_preview_value;
    ImGuiComboFlags flags = 0;

public:
    FarmSpotContainer();
    void render();
};