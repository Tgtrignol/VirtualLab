#ifndef PROCEDURE_INFORMATION_H
#define PROCEDURE_INFORMATION_H

#include <vector>
#include <string>

struct KeyPoint;
class ProcedureObject;

struct ProcedureInformation{
	std::vector<KeyPoint *> m_keyPoints;
	std::vector<ProcedureObject *> m_procedureObjects;
	std::string m_name;

	ProcedureInformation(std::vector<KeyPoint *> keyPoints, std::vector<ProcedureObject *> procedureObjects, std::string name) :
		m_keyPoints(keyPoints), m_procedureObjects(procedureObjects), m_name(name) { }

	void processInput(ControlEnum controlEnum);
};

#endif