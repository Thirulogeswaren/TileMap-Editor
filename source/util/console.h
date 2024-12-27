#ifndef CONSOLE_LOGGER_FILE_H
#define CONSOLE_LOGGER_FILE_H

enum MessageType {
    NORMAL = 0,
    INFO = 1,
    ERROR = 2,
};

namespace Console
{
    void Render();

    void LogMessage(MessageType type, const char* format, ...);
}

#define LOG_NORMAL(fmt, ...) Console::LogMessage(NORMAL, fmt, __VA_ARGS__)

#define LOG_ERROR(fmt, ...)  Console::LogMessage(ERROR, fmt, __VA_ARGS__)

#endif
