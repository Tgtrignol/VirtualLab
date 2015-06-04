#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

#define TO_RADIANS(X) float(X / 180.0f * 3.14159265358979323846f)

struct Control;
class ObjModel;
class btVector3;
class btVector4;

class ProcedureObject
{
private:
	unsigned int shaderID;
public:
	ObjModel *pObjModel;
	std::vector<Control *> controls;
	std::string fileName;
	std::string name;
	bool grabbed = false;
	int hydra = 0;
	bool closed = true;
	btVector3 *origin;
	btVector3 *rotation;
	btVector3 *scale;
	btVector4 *color;
	bool useColorInsteadOfTexture = false;

	ProcedureObject::ProcedureObject(std::string fileName, std::string name) : fileName(fileName), name(name) { }
	void init();
	void draw();
	void update();
	void setGravity(btVector3*);
	void rotate(bool horizontal, int degrees);
};

#endif