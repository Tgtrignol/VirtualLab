#ifndef STATIC_BOARD_H
#define STATIC_BOARD_H

class ShaderProgram;
class ObjModel;

class StaticBoard{
public:
	ObjModel *pObjModel;
	unsigned int shaderID;
	void init();
	void draw();
};

#endif