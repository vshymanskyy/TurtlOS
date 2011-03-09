#ifndef INC_LOG_HPP
#define INC_LOG_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Helpers.h"
#include "PoolAlloc.h"
#include "List.h"

#define __LOG_ENTRY_SIZE 512
#define __LOG_ENTRY_QTY 16

#define LOG_STR(val) STRINGIFY(val)
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
#define LOG_FUNC_EXIT ("Exited " LOG_FUNC)

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
	virtual ~Logger() {}
	virtual void AddEntry(LogLevel level, const char* time, const char* log, const char* msg) {
		if (level == LOG_FATAL)
			fprintf(stderr, "%s :\t%s\n", log, msg);
		else
			printf("%s :\t%s\n", log, msg);
	}
};

class FileLogger
	: public Logger
{
public:
	FileLogger(const char* filename)
		: _file (fopen(filename, "a+"))
	{
		assert(_file);
	}

	~FileLogger() {
		if(_file)
			fclose(_file);
	}

	virtual void AddEntry(LogLevel level, const char* time, const char* log, const char* msg) {
		if(_file)
			fprintf(_file, "%s :\t%s\n", log, msg);
	}
private:
	FILE* _file;
};

class MultiLogger
	: public Logger
	, public List<Logger*>
{
public:
	MultiLogger(Logger* l0 = NULL, Logger* l1 = NULL, Logger* l2 = NULL, Logger* l3 = NULL) {
		if(l0) Append(l0);
		if(l1) Append(l1);
		if(l2) Append(l2);
		if(l3) Append(l3);
	}

	~MultiLogger() {
		for(It it = First(); it != End(); ++it) {
			delete *it;
		}
	}

	virtual void AddEntry(LogLevel level, const char* time, const char* log, const char* msg) {
		for(It it = First(); it != End(); ++it) {
			(*it)->AddEntry(level, time, log, msg);
		}
	}
};


class Log {
private:
	class LogStream
		: public NonCopyable
	{
	public:
		LogStream(const Log* log, LogLevel level) : _data (new LogStreamData(log, level)) { }

		LogStream(const LogStream& log) : _data (log._data) {
			++_data->ref;
		}

		~LogStream() {
			if (!--_data->ref) {
				delete _data;
			}
		}

		#define __LOG_APPEND(fmt, ...) { const int s = __LOG_ENTRY_SIZE - _data->pos-1; if (s) { _data->pos += Min(s, snprintf(_data->getPos(), s, (fmt), __VA_ARGS__)); } return *this; }

		const LogStream& operator << (bool v) const                 __LOG_APPEND("%s ", v?"true":"false")
		const LogStream& operator << (char v) const                 __LOG_APPEND("%c", v)
		const LogStream& operator << (const char* v) const          __LOG_APPEND("%s", v?v:"<null>")
		const LogStream& operator << (const void* v) const          __LOG_APPEND("%p", v)

		const LogStream& operator << (short v) const                __LOG_APPEND("%hi ", v)
		const LogStream& operator << (int v) const                  __LOG_APPEND("%i ", v)
		const LogStream& operator << (long v) const                 __LOG_APPEND("%li ", v)
		const LogStream& operator << (long long v) const            __LOG_APPEND("%lli ", v)

		const LogStream& operator << (unsigned short v) const       __LOG_APPEND("%hu ", v)
		const LogStream& operator << (unsigned int v) const         __LOG_APPEND("%u ", v)
		const LogStream& operator << (unsigned long v) const        __LOG_APPEND("%lu ", v)
		const LogStream& operator << (unsigned long long v) const   __LOG_APPEND("%llu ", v)

		const LogStream& operator << (float v) const                __LOG_APPEND("%.3f ", v)
		const LogStream& operator << (double v) const               __LOG_APPEND("%.3f ", v)
		const LogStream& operator << (long double v) const          __LOG_APPEND("%.3Lf ", v)

		#undef __LOG_APPEND

	private:
		struct LogStreamData
			: public PoolAlloc<LogStreamData, __LOG_ENTRY_QTY>
		{
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

			~LogStreamData() {
				if (pos && log) {
					buff[pos++] = '\0';
					log->Print(level, buff);
				}
			}

			char* getPos() {
				return buff+pos;
			}
		};

		LogStreamData* _data;
	};

public:
	Log(const char* name, LogLevel level = LOG_NORM, Logger* logger = NULL);

	~Log();

	LogStream operator () (LogLevel level = LOG_NORM) const {
		return LogStream(this, level);
	}

	void operator () (const char* fmt, ...) const {
		va_list args;
		va_start(args, fmt);
		char vstr[__LOG_ENTRY_SIZE];
		vsnprintf(vstr, __LOG_ENTRY_SIZE, fmt, args);
		va_end(args);
		Print(LOG_NORM, vstr);
	}

	void operator () (LogLevel level, const char* fmt, ...) const {
		va_list args;
		va_start(args, fmt);
		char vstr[__LOG_ENTRY_SIZE];
		vsnprintf(vstr, __LOG_ENTRY_SIZE, fmt, args);
		va_end(args);
		Print(level, vstr);
	}

private:
	bool WillPrint(LogLevel level) const {
		if (level == LOG_CRIT) return true;
		if (level == LOG_FATAL) return true;
		if (level > LOG_NONE && level <= _level) return true;
		return false;
	}

	void Print(LogLevel level, const char* msg) const;

private:
	const char* _name;
	LogLevel    _level;
	Logger*		_logger;
};

class LogManager
	: public Singleton<LogManager>
{
	friend class Log;
	friend class Singleton<LogManager>;
public:
	void SetDefaultLogger(Logger* log) {
		if (_defaultLogger) {
			delete _defaultLogger;
		}
		_defaultLogger = log;
	}

	Logger* GetDefaultLogger() {
		return _defaultLogger;
	}

private:
	LogManager() : _defaultLogger (new Logger()) { }

	~LogManager() {
		if(_defaultLogger)
			delete _defaultLogger;
	}

	void AddLog(Log* log) {
		_logs.Append(log);
	}

	void RemoveLog(Log* log) {
		List<Log*>::It node = _logs.FindAfter(_logs.First(), log);
		_logs.Remove(node);
	}

private:
	Logger*			_defaultLogger;
	List<Log*>      _logs;
};

inline
Log::Log(const char* name, LogLevel level, Logger* logger)
	: _name     (name)
	, _level    (level)
	, _logger	(logger)
{
	LogManager::Get().AddLog(this);
}

inline
Log::~Log() {
	LogManager::Get().RemoveLog(this);
}

inline
void Log::Print (LogLevel level, const char* msg) const {
	if (WillPrint(level)) {
		if (_logger) {
			_logger->AddEntry(level, "", _name, msg);
		} else {
			LogManager::Get().GetDefaultLogger()->AddEntry(level, "", _name, msg);
		}

		if (level == LOG_FATAL) {
			abort();
		}
	}
}

#endif // INC_LOG_HPP
