#ifndef LOG_H
#define LOG_H

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <string.h>
#include "String.h"
#include "List.h"
#include "Pool.h"

#define __LOG_ENTRY_SIZE 512
#define __LOG_ENTRY_QTY 16

#define __LOG_STRINGIFY(x) #x
#define __LOG_MIN(a,b) (((a) < (b)) ? (a) : (b))

#define LOG_STR(val) __LOG_STRINGIFY(val)
#define LOG_VAL(val) "(" LOG_STR(val) ")" << " = " << (val)
#define LOG_LINE (__FILE__":" LOG_STR(__LINE__))

#ifdef MSVC
  #define LOG_FUNC (__FUNCTION__)
  #define LOG_FUNC_DEC (__FUNCDNAME__)
  #define LOG_FUNC_SIG (__FUNCSIG__)
#elif defined(GCC)
  #define LOG_FUNC (__func__)
  #define LOG_FUNC_DEC (__FUNCTION__)
  #define LOG_FUNC_SIG (__PRETTY_FUNCTION__)
#else
//  #error Compiler not supported
#endif

#define LOG_FUNC_ENTER ("Entered " LOG_FUNC)
#define LOG_FUNC_EXIT ("Exited" LOG_FUNC)

enum LogLevel {
    LOG_FATAL,      // Shows always, terminates application
    LOG_CRIT,       // Shows always
    LOG_NONE,       // For internal use only
    LOG_WARN,       // Shown when log level != LOG_NONE
    LOG_GEN,        // Log level 1
    LOG_NORM,       // Log level 2
    LOG_DEEP        // Log level 3
};

class Logger {
public:
    virtual void AddEntry(LogLevel level, const char* time, const char* log, const char* msg) = 0;
};

class Log;

class LogStream {
private:
    struct LogStreamData {
        int         ref;
        const Log*  log;
        LogLevel    level;
        int         pos;
        char        buff[__LOG_ENTRY_SIZE];

        LogStreamData(const Log* l, LogLevel lvl)
            : ref   (1)
            , log   (l)
            , level (lvl)
            , pos   (0)
        {}

        ~LogStreamData();

        char* getPos() {
            return buff+pos;
        }

        static Pool <LogStreamData, __LOG_ENTRY_QTY> entries;

/*
        void* operator new(size_t) {
            return entries.allocate();
        }

        void operator delete(void* ptr) {
            entries.release(ptr);
        }
*/
    };

    LogStreamData* _data;

    void operator = (const LogStream&) { }

public:
    LogStream(const Log* log, LogLevel level)
        : _data (new LogStreamData(log, level))
    {
    }

    LogStream(const LogStream& log)
        : _data (log._data)
    {
        ++_data->ref;
    }

    ~LogStream() {
        if (!--_data->ref) {
            delete _data;
        }
    }

    #define __LOG_APPEND(fmt, ...) { int s = __LOG_ENTRY_SIZE - _data->pos-1; if (s) { _data->pos += __LOG_MIN(s, snprintf(_data->getPos(), s, (fmt), __VA_ARGS__)); } return *this; }

    const LogStream& operator << (bool v) const                 __LOG_APPEND("%s", v?"true":"false")
    const LogStream& operator << (char v) const                 __LOG_APPEND("%c", v)
    const LogStream& operator << (const char* v) const          __LOG_APPEND("%s", v)
    const LogStream& operator << (const void* v) const          __LOG_APPEND("[%p]", v)

    const LogStream& operator << (short v) const                __LOG_APPEND("%hi", v)
    const LogStream& operator << (int v) const                  __LOG_APPEND("%i", v)
    const LogStream& operator << (long v) const                 __LOG_APPEND("%li", v)
    const LogStream& operator << (long long v) const            __LOG_APPEND("%lli", v)

    const LogStream& operator << (unsigned short v) const       __LOG_APPEND("%hu", v)
    const LogStream& operator << (unsigned int v) const         __LOG_APPEND("%u", v)
    const LogStream& operator << (unsigned long v) const        __LOG_APPEND("%lu", v)
    const LogStream& operator << (unsigned long long v) const   __LOG_APPEND("%llu", v)

    const LogStream& operator << (float v) const                __LOG_APPEND("%.3f", v)
    const LogStream& operator << (double v) const               __LOG_APPEND("%.3f", v)
    const LogStream& operator << (long double v) const          __LOG_APPEND("%.3Lf", v)

    #undef __LOG_APPEND
};

class Log {
    friend class LogManager;
public:
    Log(const char* name, LogLevel level = LOG_NORM);
    ~Log();

    LogStream operator () (LogLevel level = LOG_NORM) const {
        return LogStream(this, level);
    }

    void Print(LogLevel level, const char* msg) const;

    //void operator () (const char* fmt, ...) const;
    //void operator () (LogLevel level, const char* fmt, ...) const;

private:
    const char* _name;
    LogLevel    _level;
};

class LogManager {
    friend class Log;

public:
    static LogManager* Instance() {
        static LogManager* instance = new LogManager();
        return instance;
    }

    void AddLogger(Logger* log) {
        _loggers.Append(log);
    }

    void RemoveLogger(Logger* log) {
        _loggers.RemoveAt(_loggers.FindFirst(log));
    }

    void showDate(bool value = true) {
        _showDate = value;
    }

private:
    LogManager()
        : _showDate (false)
    {
    }

    void AddLog(Log* log) {
        _logs.Append(log);
    }

    void RemoveLog(Log* log) {
        _logs.RemoveAt(_logs.FindFirst(log));
    }

    void Print(const char* log, LogLevel level, const char* msg) {
        /*QString time;
        if (_showDate) {
            time += QDate::currentDate().toString("yyyy.MM.dd ");
        }
        time += QTime::currentTime().toString("HH:mm:ss.zzz");*/

        for (List<Logger*>::It i = _loggers.First(); i != _loggers.End(); ++i)  {
            _loggers[i]->AddEntry(level, "", log, msg);
        }

        if (level == LOG_FATAL) {
            //abort();
        }
    }

private:
    List<Logger*>   _loggers;
    List<Log*>      _logs;
    bool            _showDate;
};

inline
LogStream::LogStreamData::~LogStreamData() {
    if (pos && log) {
        buff[pos++] = '\0';
        log->Print(level, buff);
    }
}

inline
Log::Log(const char* name, LogLevel level)
    : _name     (name)
    , _level    (level)
{
    LogManager::Instance()->AddLog(this);
}

inline
Log::~Log() {
    LogManager::Instance()->RemoveLog(this);
}

inline
void Log::Print (LogLevel level, const char* msg) const {
    if (
        (level == LOG_CRIT || level == LOG_FATAL) ||
        (level > LOG_NONE && level <= _level)
    ) {
        LogManager::Instance()->Print(_name, level, msg);
    }
}

#endif // LOG_H