#ifndef STATIC_BOARD_H
#define STATIC_BOARD_H

class ShaderProgram;
class ObjModel;
class TextRepresentation;

class StaticBoard{
public:
	ObjModel *pObjModel;
	TextRepresentation *textRepresentation;
	unsigned int shaderID;
	void init();
	void draw();
};

#endif