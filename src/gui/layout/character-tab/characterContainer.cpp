#include "characterContainer.h"

std::string CharacterContainer::combo_preview_value = loadConfiguration("character", "class", "Warrior").c_str();


CharacterContainer::CharacterContainer() {
    flags &= ImGuiComboFlags_HeightRegular;
}

void CharacterContainer::render() {
    // TODO implement save to conf file the chosen class
    const char* items[] = {
        "Warrior", 
        "Mage"
    };

    ImGui::Text("Class: ");
    ImGui::SameLine();

    if (ImGui::BeginCombo("##characterComobo", combo_preview_value.c_str(), flags)) {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
            const bool is_selected = (combo_preview_value == items[n]);
            if (ImGui::Selectable(items[n], is_selected)) {
                combo_preview_value = items[n];
                saveConfiguration("character", "class", combo_preview_value.c_str());
            }
                

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }


    ImGui::SeparatorText("Keybinds for chosen class");
   
    if (combo_preview_value == "Warrior")
        warKeybindsContainer.render();

    else if (combo_preview_value == "Mage")
        mageKeybindsContainer.render();
}