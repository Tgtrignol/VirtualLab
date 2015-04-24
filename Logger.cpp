#include "Logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <Windows.h>

FILE *pFile = 0;

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

	va_list argptr;
	va_start(argptr, format);
	vfprintf(pFile, format, argptr);
	va_end(argptr);
}

void Logger::logToFile(std::string stringFormat, ...)
{
	if (pFile == 0)
		return;

	const char* format = stringFormat.c_str();

	va_list argptr;
	va_start(argptr, format);
	vfprintf(pFile, format, argptr);
	va_end(argptr);
}

void Logger::destroyLogger()
{
	if (pFile == 0)
		return;

	fclose(pFile);
	pFile = 0;
}