#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>
#include <ctime>
#include <sstream>

FILE *pFile = 0;
time_t timev;
struct tm *currentTime;

void Logger::initLogger()
{
	if (pFile != 0)
		return;

	CreateDirectory(LOG_FILE_DIRECTORY, NULL);
	pFile = fopen(LOG_FILE_LOCATION, "w");
}

void Logger::logToFile(const char* format, ...)
{
	if (pFile == 0)
		return;

	time(&timev);
	currentTime = localtime(&timev);

	char *stringFormat = new char[12 + strlen(format)];
	sprintf(stringFormat, "%02i:%02i:%02i: %s\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, format);

	va_list argptr;
	va_start(argptr, format);
	vfprintf(pFile, stringFormat, argptr);
	va_end(argptr);

	delete stringFormat;
}

void Logger::logToFile(std::string stringFormat, ...)
{
	if (pFile == 0)
		return;

	const char *format = stringFormat.c_str();

	va_list argptr;
	va_start(argptr, format);
	logToFile(format, argptr);
	va_end(argptr);
}

void Logger::destroyLogger()
{
	if (pFile == 0)
		return;

	fclose(pFile);
	pFile = 0;
}