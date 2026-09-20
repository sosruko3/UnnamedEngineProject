#pragma once

#include <fmt/base.h>
#include <stdint.h>

struct LoggerImpl;

enum class LogLevel : uint8_t {
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warning = 3,
    Error = 4,
    Critical = 5,
    Off = 6,
    Count = 7
};

class LogSystem {    
public:
    bool init(const char* logDir);
    void setLevel(LogLevel level);
    void setPattern(const char* pattern);
    void EnableFileLogging(const char* logDir);
    void flush_on(LogLevel level);
    void shutdown();
    
    template <typename... Args>
    void Trace(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Trace, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    void Debug(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Debug, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    void Info(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Info, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    void Warning(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Warning, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    void Error(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Error, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    void Critical(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Critical, format.get(), fmt::make_format_args(args...));
    }
private:
    void Write(LogLevel level, fmt::string_view format, fmt::format_args args);
    LoggerImpl* loggerImpl_ = nullptr;
    bool initialized_ = false;
};
