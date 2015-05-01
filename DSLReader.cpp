#include "DSLReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "ProcedureInformation.h"
#include "ProcedureObject.h"
#include "Logger.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if (item != "")
			elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::vector<std::string> DSLReader::readProcedureLocationFromFile()
{
	Logger::logToFile("DSLReader::readProcedureLocationFromFile()");

	std::vector<std::string> locationVector;

	std::string line;
	std::ifstream myfile(PROCEDURE_LOCATION_FILE_LOCATION);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line != "")
			{
				locationVector.push_back(line);
				Logger::logToFile(line);
			}
		}
		myfile.close();
	}
	else
	{
		Logger::logToFile("Unable to open: %s", PROCEDURE_LOCATION_FILE_LOCATION);
	}

	Logger::logToFile("~DSLReader::readProcedureLocationFromFile(), return size: %i", locationVector.size());
	return locationVector;
}

ProcedureInformation *DSLReader::readProcedureFromFile(std::string procedureFileLocation)
{
	Logger::logToFile("DSLReader::readProcedureFromFile, procedureFileLocation: %s", procedureFileLocation);

	std::string procedureName = "Not Implemented";
	std::vector<ProcedureObject *> objects;
	std::vector<KeyPoint *> keyPoints;

	ProcedureObject *currentProcedureObject = 0;

	std::string line;
	std::ifstream myfile(procedureFileLocation);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line.size() < 2)
				continue;

			if (line.substr(0, 2) == "//")
				continue;

			if (line == "")
				continue;
			
			std::vector<std::string> parts = split(line, ' ');
			
			if (parts[0] == "Item")
			{
				if (currentProcedureObject != 0)
					objects.push_back(currentProcedureObject);

				currentProcedureObject = new ProcedureObject(parts[1]);
			}

			if (parts[0] == "Control") //TODO: Replace with primitive later
			{
				if (currentProcedureObject == 0)
					continue;

				std::string temp;
				for (size_t i = 1; i < parts.size(); ++i)
				{
					temp += (parts[i] + (i + 1 == parts.size() ? "" : " "));
				}
				currentProcedureObject->controls.push_back(temp);
			}

			if (parts[0] == "Procedure")
			{
				if (currentProcedureObject != 0)
				{
					objects.push_back(currentProcedureObject);
					currentProcedureObject = 0;
				}
				//keyPoints.push_back(new KeyPoint());
			}

			Logger::logToFile(line);
		}
		myfile.close();
	}
	else
	{
		Logger::logToFile("Unable to open: %s", procedureFileLocation);
	}

	Logger::logToFile("~DSLReader::readProcedureFromFile, name: ", procedureName);
	return new ProcedureInformation(keyPoints, objects, procedureName);
}