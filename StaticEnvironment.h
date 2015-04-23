#pragma once
#include "GameObject.h"

class ObjModel;

class StaticEnvironment : public GameObject
{
public:
	unsigned int shaderID;
	unsigned int vbo_triangle_colors;
	ObjModel *pObjModel;
	void init();
	void draw();
};
