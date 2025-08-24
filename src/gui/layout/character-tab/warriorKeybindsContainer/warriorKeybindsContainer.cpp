#include "warriorKeybindsContainer.h"

ImGuiKey WarriorKeybindsContainer::autoAttack = loadKeybindConfiguration("warrior", "autoAttack");
ImGuiKey WarriorKeybindsContainer::battleCry= loadKeybindConfiguration("warrior", "battleCry");
ImGuiKey WarriorKeybindsContainer::defenceCall = loadKeybindConfiguration("warrior", "defenceCall");

void WarriorKeybindsContainer::render() {
    static ImGuiKey* keybinds[3] = { &autoAttack, &battleCry, &defenceCall };
    static bool isPressed[3] = { false, false, false };
    static bool bindAlreadySet[3] = { false, false, false };
    const char* labels[3] = {
        "Auto attack bind:",
        "Battle cry bind:",
        "Defence call bind:"
    };
    const char* ids[3] = {
        "WarriorAutoAttackBind",
        "WarriorBattleCryBind",
        "WarriorDefenceCallBind"
    };

    static bool bindChanged = false;

    if (ImGui::BeginTable("WarriorKeybindTable", 3, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Bind", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Error", ImGuiTableColumnFlags_WidthFixed, 150.0f);  // Terza colonna fissa

        for (int i = 0; i < 3; i++) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushID(ids[i]);
            ImGui::TextUnformatted(labels[i]);
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

            ImGui::PopID();

            ImGui::TableNextColumn();
            if (bindAlreadySet[i]) {
                ImGui::TextColored(ImVec4(0.980f, 0.153f, 0.153f, 1.0f), "Choose another key");
            }
        }

        ImGui::EndTable();
    }

    if (bindChanged) {
        if (ImGui::Button("Save")) {
            saveConfiguration("warrior", "autoAttack", ImGui::GetKeyName(autoAttack));
            saveConfiguration("warrior", "battleCry", ImGui::GetKeyName(battleCry));
            saveConfiguration("warrior", "defenceCall", ImGui::GetKeyName(defenceCall));

            bindChanged = false;
        }

        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.1f, 0.5f, 1.0f, 1.0f), "Press the button to save the keybinds");
    }
}