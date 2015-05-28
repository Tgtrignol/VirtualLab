#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

struct Control;
class ObjModel;
class btVector3;

class ProcedureObject
{
private:
	unsigned int shaderID;
	ObjModel *pObjModel;
public:
	std::vector<Control *> controls;
	std::string fileName;
	std::string name;
	btVector3 *origin;

	ProcedureObject::ProcedureObject(std::string fileName, std::string name) : fileName(fileName), name(name) { }
	void init();
	void draw();
	void update();
};

#endif