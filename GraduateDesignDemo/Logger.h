#pragma once

#include <vector>
#include <string>

enum LOG_TYPE
{
	LOG_DEFAULT = 0,
	LOG_DEBUG = 2,
	LOG_INIT = 4,
	LOG_WARNING = 8,
	LOG_ERROR = 16,
};

class Log
{
private:

	static std::vector<std::vector<std::string>> m_LogEntries;

	// stores the filter settings e.g. which log types the user likes to display.
	static unsigned int m_Filter;

public:
	// logs a user/engine specified message with optional type of log message. Note that log
	// messages are queued and flushed/displyayed once by calling Flush.
	static void Message(const std::string& message, const LOG_TYPE type = LOG_DEFAULT);

	// through bitwise concatanation of LOG_TYPEs, specify which log messages should print/write
	// to std output or file.
	static void SetFilter(unsigned int filter);

	// clear all log entries
	static void Clear();

	// displays all log entries to the standard output. By passing a log type one can filter on
	// specific log types.
	static void Display(const LOG_TYPE type = LOG_DEFAULT);

	// TODO(Joey): implement file IO

private:
	static void printMessage(const std::string& message, const LOG_TYPE type);
};
