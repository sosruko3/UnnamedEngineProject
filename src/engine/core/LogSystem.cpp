#include "LogSystem.h"

// this deletes lower priority log messages. we don't see debug messages on release.
#ifdef NDEBUG
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include "spdlog/common.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>
#include <assert.h>

// constants
constexpr size_t LOG_MAX_FILE_SIZE = static_cast<size_t>(5 * 1024 * 1024);
constexpr size_t LOG_MAX_FILES = 3;
constexpr const char* LOG_FILE_NAME = "engine.log";

// this for opaque logger implementation. less header dependencies.
struct LoggerImpl {
    std::shared_ptr<spdlog::logger> spdlogger;
};

LoggerImpl* LogSystem::loggerImpl_ = nullptr;


bool LogSystem::init(const char* logDir) {
    if (loggerImpl_ != nullptr) {
        return false;
    }
    
    // create logger implementation.
    loggerImpl_ = new LoggerImpl();

    // initialize thread pool.
    spdlog::init_thread_pool(8192, 1);
    
    // enable console and file logging.
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        std::string(logDir) + LOG_FILE_NAME, LOG_MAX_FILE_SIZE, LOG_MAX_FILES);
    
    // this is async logger with overrun oldest, so old logs are discarded if the queue is full.
    loggerImpl_->spdlogger = std::make_shared<spdlog::async_logger>(
        "engine", spdlog::sinks_init_list{ consoleSink, fileSink },
        spdlog::thread_pool(),
        spdlog::async_overflow_policy::overrun_oldest);

    // set log level and pattern.
    loggerImpl_->spdlogger->set_level(spdlog::level::info);
    loggerImpl_->spdlogger->set_pattern("%Y-%m-%d %H:%M:%S] [%^%l%$] %v");

    // flush on error level, so errors and higher priorities are immediately visible.
    LogSystem::flush_on(LogLevel::Error);
    return true;
}

void LogSystem::Write(LogLevel level, fmt::string_view fmt, fmt::format_args args) {
    if (!loggerImpl_ || !loggerImpl_->spdlogger) return;
    // log level is checked before formatting.
    auto spdLevel = static_cast<spdlog::level::level_enum>(level);
    // should_log is checked.
    if (!loggerImpl_->spdlogger->should_log(spdLevel)) return;
    
    // stack buffer, zero heap allocations for standard messages
    fmt::memory_buffer buf;
    // fmt::vformat_to is used to avoid heap allocations for the formatted string.
    fmt::vformat_to(std::back_inserter(buf), fmt, args);
    // log(level, string_view) is called with the formatted string.
    loggerImpl_->spdlogger->log(spdLevel, std::string_view(buf.data(), buf.size()));
}

void LogSystem::setLevel(LogLevel level) {
    loggerImpl_->spdlogger->set_level(static_cast<spdlog::level::level_enum>(level));
}

void LogSystem::setPattern(const char* pattern) {
    loggerImpl_->spdlogger->set_pattern(pattern);
}

void LogSystem::flush_on(LogLevel level) {
    loggerImpl_->spdlogger->flush_on(static_cast<spdlog::level::level_enum>(level));
}

void LogSystem::shutdown() {
    if (loggerImpl_ == nullptr) {
        return;
    }
    // force flush and shutdown.
    loggerImpl_->spdlogger->flush();
    spdlog::shutdown();
    delete loggerImpl_;
    loggerImpl_ = nullptr;
}
