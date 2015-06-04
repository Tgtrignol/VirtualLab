#ifndef STATIC_LAB_ENVIRONMENT_H
#define STATIC_LAB_ENVIRONMENT_H

class ShaderProgram;
class ObjModel;

class StaticLabEnvironment{
public:
	ObjModel *pObjModel;
	unsigned int shaderID;
	void init();
	void draw();
};

#endif