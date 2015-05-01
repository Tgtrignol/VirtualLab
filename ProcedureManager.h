#ifndef PROCEDURE_MANAGER_H
#define PROCEDURE_MANAGER_H

#include <vector>
#include <string>

struct ProcedureInformation;

class ProcedureManager{
public:
	void init();
	void draw();
	void update();
private:
	std::vector<std::string> procedureLocations;
	std::vector<ProcedureInformation *> procedureInformations;
};

#endif