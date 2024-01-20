#include "console.h"

#include "imgui.h"

namespace
{
    constexpr size_t buffer_size { 64 };

    ImVector<char[buffer_size]> history;
    char temp_buffer[buffer_size];
}

constexpr int flags { ImGuiInputTextFlags_EnterReturnsTrue };

void Console::Render()
{
    ImGui::Begin("console logs");

    if (ImGui::Button("clear")) { history.clear(); }

    ImGui::SeparatorText("LOGS");
    ImGui::BeginChild("##", ImVec2{ 300.0f, 360.0f });

    for (const auto& message : history)
        ImGui::Text(message);

    ImGui::EndChild();

    ImGui::Separator();
    memset(&temp_buffer, 0, buffer_size);
    if (ImGui::InputText("##", temp_buffer, sizeof(temp_buffer), flags)) {
        history.push_back(temp_buffer);
    }
    
    ImGui::End();
}

void Console::LogMessage(const char* msg)
{
    memcpy(&temp_buffer, msg, buffer_size);
    history.push_back(temp_buffer);
}
