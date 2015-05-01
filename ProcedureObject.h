#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

class ObjModel;

class ProcedureObject
{
private:
	unsigned int shaderID;
	ObjModel *pObjModel;
public:
	ProcedureObject(std::string name);
	void init();
	void draw();
	void update();
	std::vector<std::string> controls;
};

#endif