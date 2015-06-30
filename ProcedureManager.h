#ifndef PROCEDURE_MANAGER_H
#define PROCEDURE_MANAGER_H

#include <vector>
#include <string>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "DSLReader.h"

enum ControlEnum;
struct ProcedureInformation;
class ProcedureObject;

class ProcedureManager{
public:
	void init();
	void draw();
	void update(ControlEnum controlEnum);
	ProcedureObject* getLeftGrabbedObject();
	ProcedureObject* getRightGrabbedObject();
	static ProcedureManager* getInstance();

	ProcedureObject *righternSelectedProcedureObject;
	ProcedureObject *lefternSelectedProcedureObject;
	std::string RightLeft = "None";
	DSLReader* dslReader = new DSLReader();

private:
	std::vector<std::string> procedureFileLocations;
	ProcedureInformation *currentProcedureInformation;
	ProcedureObject* getGrabbedObject(string Side);
	bool procedure = true;
	bool anglePositive = true;
	bool changingObjectTest = false;

};

#endif