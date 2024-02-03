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

    void LogMessage(const char*, MessageType = NORMAL);

    void LogMessage(const char*, const char*, MessageType = NORMAL);
}

#endif
