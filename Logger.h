#ifndef LOGGER_H
#define LOGGER_H

#define LOG_FILE_LOCATION "C:\\VirtualLab\\LogFile.txt"
#define LOG_FILE_DIRECTORY L"C:\\VirtualLab"

#include <string>

class Logger{
public:
	static void initLogger();
	static void logToFile(const char* format, ...);
	static void logToFile(std::string stringFormat, ...);
	static void destroyLogger();
};

#endif