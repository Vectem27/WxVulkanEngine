#include "Logger.h"

LogCategory LogDefault{"LogDefault"};

void Log(LogLevel level, LogCategory logCategory, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    Logger::GetLogger().Log(level, logCategory, message, args);
    va_end(args);
}

Logger* Logger::instance{nullptr};

Logger::Logger()
{
    OverridePrintToLogFunction(
        [](LogLevel level, std::string message)
        {
            const std::string WHITE   = "\033[0m" ;
            const std::string RED     = "\033[31m";
            const std::string GREEN   = "\033[32m";
            const std::string YELLOW  = "\033[33m";
            const std::string BLUE    = "\033[34m";
            const std::string MAGENTA = "\033[35m";
            const std::string ORANGE  = "\033[38;5;214m";
            const std::string GRAY   = "\033[38;5;235m";

            std::string color;

            switch (level)
            {
            case Trace:    color = GRAY  ; break;
            case Debug:    color = GREEN  ; break;
            case Info:     color = WHITE  ; break;
            case Warning:  color = ORANGE  ; break;
            case Error:    color = RED    ; break;
            case Critical: color = MAGENTA    ; break;
            }

            std::cout << color << message << WHITE << "\n";
        }
    );
}

void Logger::Log(LogLevel level, LogCategory logCatgory, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    Log(level, logCatgory, message, args);
    va_end(args);
}

void Logger::Log(LogLevel level, LogCategory logCatgory, const char *message, va_list args)
{
    char buffer[2048];

    vsnprintf(buffer, sizeof(buffer), message, args);

    std::ostringstream logStream;
    switch (level)
    {
    case Trace: logStream    << "[TRACE] "   ; break;
    case Debug: logStream    << "[DEBUG] "   ; break;
    case Info: logStream     << "[INFO] "    ; break;
    case Warning: logStream  << "[WARNING] " ; break;
    case Error: logStream    << "[ERROR] "   ; break;
    case Critical: logStream << "[CRITICAL] "; break;
    }
    logStream << logCatgory << ": " << buffer;

    PrintLog(level, logStream.str());
}

void Logger::PrintLog(LogLevel level, std::string message)
{
    switch (level)
    {
    case Trace:    if (verbosity <= 4) return; break;
    case Debug:    if (verbosity <= 3) return; break;
    case Info:     if (verbosity <= 2) return; break;
    case Warning:  if (verbosity <= 1) return; break;
    case Error:    if (verbosity == 0) return; break;
    case Critical: if (verbosity == 0) return; break;
    }

    if (printToLog)
        printToLog(level, message);
}
