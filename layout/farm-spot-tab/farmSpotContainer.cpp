#include "farmSpotContainer.h"

std::string FarmSpotContainer::combo_preview_value = loadConfiguration("farmspot", "spot", "Emerald Cave - Goblin spot").c_str();

FarmSpotContainer::FarmSpotContainer() {
    flags &= ImGuiComboFlags_HeightRegular;
}

void FarmSpotContainer::render() {
    const char* items[] = {
        "Emerald Cave - Goblin spot", 
        "Rayfort - Avian Colony - Right Spot", 
        "Rayfort - Avian Colony - Left Spot"
    };

    /*
    For every spot add description of the spot 
    like drops of the mobs, 
    mobs, 
    avarage gold, 
    average time for full the inventory,
    average time for lottery
    */

    ImGui::Text("Farm Spot: ");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##farmSpotComobo", combo_preview_value.c_str(), flags)) {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
            const bool is_selected = (combo_preview_value == items[n]);
            if (ImGui::Selectable(items[n], is_selected)) {
                combo_preview_value = items[n];
                saveConfiguration("farmspot", "spot", combo_preview_value.c_str());
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}