#pragma once
#include "ProcedureObject.h"

class ObjModel;

//////////////////////////////
//EXAMPLE ONLY, REMOVE LATER//
//////////////////////////////

class StaticEnvironment : public ProcedureObject
{
public:
	unsigned int shaderID;
	unsigned int vbo_triangle_colors;
	ObjModel *pObjModel;
	void init();
	void draw();
};
