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
	std::string pressedSide = "None";
	DSLReader* dslReader = new DSLReader();
	int procedureNumber = 0;

private:
	std::vector<std::string> procedureFileLocations;
	ProcedureInformation *currentProcedureInformation;
	ProcedureObject* getGrabbedObject(std::string Side);
	bool doingProcedure = true;
	bool anglePositive = true;
	bool changingObjectTest = false;

};

#endif