#include "mageKeybindsContainer.h"

ImGuiKey MageKeybindsContainer::autoAttack = loadKeybindConfiguration("mage", "autoAttack");
ImGuiKey MageKeybindsContainer::frostPrison = loadKeybindConfiguration("mage", "frostPrison");
ImGuiKey MageKeybindsContainer::iceRain = loadKeybindConfiguration("mage", "iceRain");
ImGuiKey MageKeybindsContainer::auraOfIciness = loadKeybindConfiguration("mage", "auraOfIciness");
ImGuiKey MageKeybindsContainer::captivatingWhirlwind = loadKeybindConfiguration("mage", "captivatingWhirlwind");
ImGuiKey MageKeybindsContainer::manaProtectiveBarier = loadKeybindConfiguration("mage", "manaProtectiveBarier");
ImGuiKey MageKeybindsContainer::concentration = loadKeybindConfiguration("mage", "concentration");

void MageKeybindsContainer::render() {
    static ImGuiKey* keybinds[7] = {
        &autoAttack,
        &frostPrison,
        &iceRain,
        &auraOfIciness,
        &captivatingWhirlwind,
        &manaProtectiveBarier,
        &concentration
    };
    static bool isPressed[7] = { false, false, false, false, false, false, false };
    static bool bindAlreadySet[7] = { false, false, false, false, false, false, false };
    const char* labels[7] = {
        "Auto attack bind: ",
        "Frost prison bind: ",
        "Ice rain bind: ",
        "Aura of iciness bind: ",
        "Whirlwind bind: ",
        "Protective barrier bind: ",
        "Concentration: "
    };
    const char* ids[7] = {
        "MageAutoAttackBind",
        "MageFrostPrisonBind",
        "MageIceRainBind",
        "MageAuraOfIcinessBind",
        "MageCaptivatingWhirlwindBind",
        "MageManaProtectiveBarierBind",
        "MageConcentrationBind"
    };

    static bool bindChanged = false;

    if (ImGui::BeginTable("MageKeybindTable", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)) {
        // Colonna 1 = testo variabile, Colonna 2 = larghezza fissa 150
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Bind", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Error", ImGuiTableColumnFlags_WidthFixed, 150.0f);

        for (int i = 0; i < 7; i++) {
            ImGui::TableNextRow();

            // Colonna testo
            ImGui::TableNextColumn();
            ImGui::PushID(ids[i]);
            ImGui::TextUnformatted(labels[i]);

            // Colonna bottone
            ImGui::TableNextColumn();
            const char* displayText = isPressed[i] ? "Press a key..."
                : *keybinds[i] == ImGuiKey_None ? "None" : ImGui::GetKeyName(*keybinds[i]);

            if (ImGui::Button(displayText, ImVec2(-FLT_MIN, 0)))
                isPressed[i] = true;

            if (isPressed[i]) {
                for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
                    if (ImGui::IsKeyPressed((ImGuiKey)key)) {
                        isPressed[i] = false;
                        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
                            break;

                        if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft) || ImGui::IsKeyPressed(ImGuiKey_MouseRight) || ImGui::IsKeyPressed(ImGuiKey_MouseMiddle))
                            break;

                        if (containsKeybind(*keybinds, (ImGuiKey)key))
                            bindAlreadySet[i] = true;
                        else {
                            *keybinds[i] = (ImGuiKey)key;
                            bindAlreadySet[i] = false;
                            bindChanged = true;
                        }
                    }
                }
            }

            ImGui::TableNextColumn();
            if (bindAlreadySet[i]) {
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Choose another key");
            }

            ImGui::PopID();
        }

        ImGui::EndTable();
    }

    if (bindChanged) {
        if (ImGui::Button("Save")) {
            saveConfiguration("mage", "autoAttack", ImGui::GetKeyName(autoAttack));
            saveConfiguration("mage", "frostPrison", ImGui::GetKeyName(frostPrison));
            saveConfiguration("mage", "iceRain", ImGui::GetKeyName(iceRain));
            saveConfiguration("mage", "auraOfIciness", ImGui::GetKeyName(auraOfIciness));
            saveConfiguration("mage", "captivatingWhirlwind", ImGui::GetKeyName(captivatingWhirlwind));
            saveConfiguration("mage", "manaProtectiveBarier", ImGui::GetKeyName(manaProtectiveBarier));
            saveConfiguration("mage", "concentration", ImGui::GetKeyName(concentration));

            bindChanged = false;
        }
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 0.4f, 1.0f, 1.0), "Press the button to save the keybinds");
    }
}