#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

struct Control;
class ObjModel;

class ProcedureObject
{
private:
	unsigned int shaderID;
	ObjModel *pObjModel;
public:
	std::vector<Control *> controls;
	std::string id;
	std::string name;

	ProcedureObject(std::string id, std::string name);
	void init();
	void draw();
	void update();
};

#endif