#pragma once
#include "String.h"
#include "List.h"
#include "Console.h"

class Log
{
public:
	/// Constructor
	Log(const String& name, bool enabled = false)
		: mName(name)
		, mEnabled(enabled)
	{
	}

	/// Destructor
	~Log() {
	}

	/// Logs a message
	Log& operator << (const String& msg) {
		if (mEnabled) {
			console << mName << ": " << msg << endl;
		}
	}

	bool IsEnabled() {
		return mEnabled;
	}

	void Enable(bool enable = true) {
		mEnabled = enable;
	}

private:
	String mName;
	bool mEnabled;
};


class LogManager
{
private:
	List<Log> mLogs;

	LogManager() {
	}

	~LogManager() {
	}

public:
	LogManager* Instance() {
		static LogManager* instance = new LogManager();
		return instance;
	}

	void EnableLog(const String& name) {

	}
};

