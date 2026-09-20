#pragma once

#include <fmt/base.h>
#include <stdint.h>

struct LoggerImpl;

// this is identical to spdlog's LogLevel enum
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
    static bool init(const char* logDir);
    static void setLevel(LogLevel level);
    static void setPattern(const char* pattern);
    static void EnableFileLogging(const char* logDir);
    static void flush_on(LogLevel level);
    static void shutdown();
    
    template <typename... Args>
    static void Trace(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Trace, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    static void Debug(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Debug, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    static void Info(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Info, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    static void Warning(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Warning, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    static void Error(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Error, format.get(), fmt::make_format_args(args...));
    }

    template <typename... Args>
    static void Critical(fmt::format_string<Args...> format, Args&&... args) {
        Write(LogLevel::Critical, format.get(), fmt::make_format_args(args...));
    }
private:
    static void Write(LogLevel level, fmt::string_view format, fmt::format_args args);
    static LoggerImpl* loggerImpl_;
};
