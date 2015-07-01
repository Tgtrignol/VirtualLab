#ifndef STATIC_MENU_H
#define STATIC_MENU_H

class ShaderProgram;
class ObjModel;

class StaticMenu{
public:
	ObjModel *pObjModel;
	unsigned int shaderID;
	void init();
	void draw();
};

#endif