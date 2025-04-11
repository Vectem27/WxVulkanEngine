#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h> 

#include <sstream>
#include <iostream>

#include <functional>

enum LogLevel
{
    /* Low level details*/
    Trace,
    /* Debug details */
    Debug,
    /* Information */
    Info,
    /* Warning */
    Warning,
    /* Error */
    Error,
    /* Critical error */
    Critical
};

typedef std::string LogCategory;


void Log(LogLevel level, LogCategory logCategory, const char* message, ...);

extern LogCategory LogDefault;


class Logger
{
private:
    Logger();
public:
    static inline Logger& GetLogger() 
    { 
        if (instance == nullptr)
            instance = new Logger();
        return *instance; 
    }

    void Log(LogLevel level, LogCategory logCatgory, const char* message, ...);
    void Log(LogLevel level, LogCategory logCatgory, const char* message, va_list args);

    void SetVerbosity(uint8_t verbosity) { this->verbosity = verbosity <= 5 ? verbosity : 5; }
    uint8_t GetVerbosity() const { return verbosity; }

    void OverridePrintToLogFunction(std::function<void(LogLevel, std::string)> newFunction) {printToLog = newFunction; }
private:
    void PrintLog(LogLevel level, std::string message);

    uint8_t verbosity{3};
    std::function<void(LogLevel, std::string)> printToLog;

    static Logger* instance;
};


#endif // LOGGER_H