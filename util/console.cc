#include "console.h"

#include "imgui.h"

namespace {
    constexpr size_t buffer_size = 128ull;
    char temp_buffer[buffer_size];

    struct ActualMessage {
        char data[buffer_size];
        MessageType type;

        ActualMessage(const char* _data) : ActualMessage{ _data, NORMAL } { }

        ActualMessage(const char* _data, MessageType _type) : type{ _type } {
            memcpy(&this->data, _data, buffer_size);
            memset(&temp_buffer, 0, buffer_size);
        }
    };

    ImVector<ActualMessage> history{};
}

constexpr int flags { ImGuiInputTextFlags_EnterReturnsTrue };

void Console::Render()
{
    ImGui::SetNextWindowSize(ImVec2{ 300.0f, 390.0f });
    ImGui::Begin("Console Logs");
    
    ImGui::Spacing();
    
    if (ImGui::InputText("##", temp_buffer, buffer_size, flags)) {
        history.push_back(temp_buffer);
    }

    ImGui::SameLine();

    if (ImGui::Button("clear", { 66.0f, 24.0f })) {
        history.clear();
    }

    ImGui::SeparatorText("LOGS");
    ImGui::BeginChild("##");

    for (const auto& [message, type] : history) {
        if (type == INFO)
        {
            ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 0.0f, 1.0f }, message);
        }
        else if (type == ERROR)
        {
            ImGui::TextColored(ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f }, message);
        }
        else {
            ImGui::Text(message); 
        }
    }

    ImGui::EndChild();

    ImGui::End();
}

void Console::LogMessage(const char* msg, MessageType type)
{
    history.push_back(ActualMessage{ msg, type });
}

void Console::LogMessage(const char* msg, const char* opt, MessageType type)
{
    memcpy(&temp_buffer, msg, buffer_size);
    strcat(temp_buffer, opt);
    history.push_back(ActualMessage{ temp_buffer, type });
}
