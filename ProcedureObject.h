#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

class ObjModel;

class ProcedureObject
{
private:
	unsigned int shaderID;
	ObjModel *pObjModel;
public:
	void init();
	void draw();
	void update();
};

#endif