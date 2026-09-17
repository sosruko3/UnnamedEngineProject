#include "LogSystem.h"
#include "fmt/base.h"
#include "fmt/chrono.h" // IWYU pragma: keep
// fmt/chrono.h is getting false positive warnings
#include <stdio.h>
// THERE are a few serious issues on this code. FIX ASAP!

static constexpr const char *LOG_TAGS[] = {
  "[INFO]" , "[WARNING]",
  "[ERROR]", "[DEBUG]"
};

static FILE *logFile = nullptr;
void Logger_WriteToFile(const char *finalMessage, LogLevel level) {
  // NOTE: This function is not thread-safe right now.

  fputs(finalMessage, stdout);

  if (logFile) {
    fputs(finalMessage, logFile);

    if (level == LogLevel::Warning || level == LogLevel::Error) {
      fflush(logFile);
    }
  }
}

// Might think about making this a bool return type. For safety.
void Logger_Init(const char* PrefPath) {
  // Get file directory
  const char *appDir = PrefPath;

  char pathBuffer[512] = {};
  // 512 char should be theoretically enough for any appDir path but not %100 safe.

  // Creates logs folder. This part depends on appDir location.
  // This might mean different things depending on whether '/' used or not.
  fmt::format_to_n(pathBuffer, sizeof(pathBuffer) - 1, "{}", appDir);

  // Build the full filename
  fmt::format_to_n(pathBuffer, sizeof(pathBuffer) - 1, "{}/game.log",
                   appDir);

  logFile = fopen(pathBuffer, "a");
  if (logFile) {
    Log(LogLevel::Info, "Logger Initialized.");
  } else {
    // If error , try to save in root folder
    logFile = fopen("game_fallback.log", "w");
    // NOTE: There is no game_fallback.log implementation yet.
    fmt::print(
        stderr,
        "ERROR: Could not create log file in logs folder. Trying fallback.\n");
  }
}

void Logger_Shutdown(void) {
  if (logFile) {
    fputs("Logger Shutdown.", logFile);
    fclose(logFile);
    logFile = nullptr;
  }
}

void Logger_LogImplementation(LogLevel level, fmt::string_view format,
                              fmt::format_args args) {
  char buffer[1024] = {};
  // This works for now due to zero-initialization but might break in the future.
  // Better manually add null-termination.

  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  // -2 is for \n. In case buffer is full.
  // This part checks time and calculates DDMMYY,HMS everytime.
  // Which is a waste, might change some things here.
    fmt::format_to_n_result<char *> res1 =
      fmt::format_to_n(buffer, sizeof(buffer) - 2, "{:%d/%m/%Y %H:%M:%S} {} ",
                       std::chrono::time_point_cast<std::chrono::seconds>(now), 
                       LOG_TAGS[static_cast<uint8_t>(level)]
                      );

  size_t written = (res1.size > sizeof(buffer) - 2) ? (sizeof(buffer) - 2) : res1.size;
  fmt::format_to_n_result<char *> res2 =
      fmt::vformat_to_n(buffer + written, sizeof(buffer) - 2 - written, format, args);
  fmt::format_to_n(res2.out, 1, "\n");
  Logger_WriteToFile(buffer, level);
}
