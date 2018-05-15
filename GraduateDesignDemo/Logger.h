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

	static unsigned int m_Filter;

public:

	static void Message(const std::string& message, const LOG_TYPE type = LOG_DEFAULT);

	static void SetFilter(unsigned int filter);

	static void Clear();

	static void Display(const LOG_TYPE type = LOG_DEFAULT);

private:
	static void printMessage(const std::string& message, const LOG_TYPE type);
};
