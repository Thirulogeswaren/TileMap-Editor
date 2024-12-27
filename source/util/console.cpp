#include "console.h"

#include "imgui.h"
#include <cstdio>

namespace {
    constexpr size_t buffer_size = 128ull;

    struct ActualMessage {
        char data[buffer_size];
        MessageType type;

        ActualMessage(const char* _data, va_list arg_list, MessageType _type)
            : type{ _type } {
            vsnprintf(data, buffer_size, _data, arg_list);
        }
    };

    ImVector<ActualMessage> history;
}

void Console::Render()
{
    ImGui::SetNextWindowSize(ImVec2{ 300.0f, 390.0f });
    ImGui::Begin("Console Logs");
    
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

    ImGui::End();
}

void Console::LogMessage(MessageType type, const char* format, ...)
{
    va_list arg_list;
    va_start(arg_list, format);

    history.push_back(ActualMessage{ format, arg_list, type });

    va_end(arg_list);
}
