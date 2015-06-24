#ifndef PROCEDURE_MANAGER_H
#define PROCEDURE_MANAGER_H

#include <vector>
#include <string>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

enum ControlEnum;
struct ProcedureInformation;
class ProcedureObject;

class ProcedureManager{
public:
	void init();
	void draw();
	void update(ControlEnum controlEnum);
private:
	std::vector<std::string> procedureFileLocations;
	ProcedureInformation *currentProcedureInformation;
	bool procedure = true;
	bool anglePositive = true;
	bool changingObjectTest = false;
public:
	ProcedureObject *righternSelectedProcedureObject;
	ProcedureObject *lefternSelectedProcedureObject;
	std::string RightLeft = "None";
};

#endif