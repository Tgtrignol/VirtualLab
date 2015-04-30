#include "DSLReader.h"
#include "ProcedureInformation.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Logger.h"

std::vector<std::string> DSLReader::readProcedureLocationFromFile()
{
	Logger::logToFile("DSLReader::readProcedureLocationFromFile()");

	std::string line;
	std::ifstream myfile(PROCEDURE_LOCATION_FILE_LOCATION);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			Logger::logToFile(line);
		}
		myfile.close();
	}
<<<<<<< HEAD
	//else Logger::logToFile("Unable to open %s", "test");
=======
	else
	{
		Logger::logToFile("Unable to open: %s", PROCEDURE_LOCATION_FILE_LOCATION);
	}
>>>>>>> b0cba0c... Fixed mistake in Logger.

	Logger::logToFile("~DSLReader::readProcedureLocationFromFile()");
	return std::vector<std::string>();
}

ProcedureInformation *DSLReader::readProcedureFromFile(std::string procedureFileLocation)
{
	return new ProcedureInformation(std::vector<KeyPoint *>(), std::vector<ProcedureObject *>(), "");
}