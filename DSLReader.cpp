#include "DSLReader.h"
#include "ProcedureInformation.h"

std::vector<std::string> DSLReader::readProcedureLocationFromFile()
{
	return std::vector<std::string>();
}

ProcedureInformation *DSLReader::readProcedureFromFile(std::string procedureFileLocation)
{
	return new ProcedureInformation(std::vector<KeyPoint *>(), std::vector<ProcedureObject *>(), "");
}