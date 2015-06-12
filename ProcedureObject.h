#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

#define TO_RADIANS(X) float(X / 180.0f * 3.14159265358979323846f)

struct Control;
class ObjModel;
class btVector3;
class btVector4;

enum OriginAnchor{
	Table,
	Room
};

std::string originAnchorToString(OriginAnchor originAnchor);
OriginAnchor stringToOriginAnchor(std::string str);


class ProcedureObject
{
private:
	unsigned int shaderID;
	void applyOriginAnchorTranslation();
public:
	ObjModel *pObjModel;
	std::vector<Control *> controls;
	std::string fileName;
	std::string name;
	bool grabbed = false;
	bool horizontal = false;
	std::string LeftRight = "None";
	bool closed = true;
	btVector3 *origin;
	bool initRigidbodies = false;
	btVector3 *rotation;
	btVector3 *scale;
	btVector4 *color;
	bool useColorInsteadOfTexture = false;
	OriginAnchor originAnchor;

	ProcedureObject::ProcedureObject(std::string fileName, std::string name) : fileName(fileName), name(name) { }
	void init();
	void draw();
	void update();
	void rotate(std::string direction, int degrees);
};

#endif