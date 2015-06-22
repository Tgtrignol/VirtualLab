#include "DSLReader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "ProcedureInformation.h"
#include "ProcedureObject.h"
#include "Logger.h"
#include "KeyPoint.h"
#include "Control.h"
#include "ControlEnum.h"
#include <btBulletCollisionCommon.h>

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

	std::string procedureName;
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
			
			std::vector<std::string> parts = split(line, ' ');

			if (parts[0] == "Name")
			{
				std::replace(parts[1].begin(), parts[1].end(), '_', ' ');
				procedureName = parts[1];
			}
			else
			if (parts[0] == "Item")
			{
				if (currentProcedureObject != 0)
					objects.push_back(currentProcedureObject);

				currentProcedureObject = new ProcedureObject(parts[2], parts[1]);

				currentProcedureObject->changeObject = parts[3] == "1" ? true : false;

				currentProcedureObject->originAnchor = stringToOriginAnchor(parts[4]);

				int waterCompensation = 0;
				if (currentProcedureObject->originAnchor == OriginAnchor::Error)
				{
					waterCompensation = 6;
					currentProcedureObject->useWaterOverlay = true;
					currentProcedureObject->waterDirectionMin = new btVector3(std::stof(parts[4]), std::stof(parts[5]), std::stof(parts[6]));
					currentProcedureObject->waterDirectionMax = new btVector3(std::stof(parts[7]), std::stof(parts[8]), std::stof(parts[9]));
					currentProcedureObject->originAnchor = stringToOriginAnchor(parts[4 + waterCompensation]);
				}

				currentProcedureObject->origin = new btVector3(std::stof(parts[5 + waterCompensation]), std::stof(parts[6 + waterCompensation]), std::stof(parts[7 + waterCompensation]));
				currentProcedureObject->rotation = new btVector3(std::stof(parts[8 + waterCompensation]), std::stof(parts[9 + waterCompensation]), std::stof(parts[10 + waterCompensation]));
				currentProcedureObject->scale = new btVector3(std::stof(parts[11 + waterCompensation]), std::stof(parts[12 + waterCompensation]), std::stof(parts[13 + waterCompensation]));

				if (parts.size() > (14 + waterCompensation))
				{
					currentProcedureObject->useColorInsteadOfTexture = true;
					currentProcedureObject->color = new btVector4(std::stof(parts[14 + waterCompensation]), std::stof(parts[15 + waterCompensation]), std::stof(parts[16 + waterCompensation]), std::stof(parts[17 + waterCompensation]));
				}
			}
			else
			if (parts[0] == "Control")
			{
				if (currentProcedureObject == 0)
					continue;

				currentProcedureObject->controls.push_back(new Control(StringToControlEnum(parts[1]), parts[2]));
			}
			else
			if (parts[0] == "Procedure")
			{
				if (currentProcedureObject != 0)
				{
					objects.push_back(currentProcedureObject);
					currentProcedureObject = 0;
				}

				std::replace(parts[1].begin(), parts[1].end(), '_', ' ');
				std::vector<std::string *> params;
				for (size_t i = 3; i < parts.size(); i++)
				{
					params.push_back(new std::string(parts[i]));
				}
				keyPoints.push_back(new KeyPoint(parts[1], false, parts[2], params));
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