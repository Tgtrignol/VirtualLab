#ifndef TEXT_REPRESENTATION_H
#define TEXT_REPRESENTATION_H
#include <vector>
#include <stdlib.h>
#include <glut\glut.h>

class TextRepresentation {

public:
	TextRepresentation();
	~TextRepresentation();

	void drawNotes(const char *text, int length);
	void drawText(const char*, int, int, int, int);
	void drawText(const char *text, int length, int x, int y, int z, GLfloat r, GLfloat g, GLfloat b);
	void drawList(std::vector<std::string>);
	void drawList(std::vector<std::string> list, int x, int y, int z);
	void draw();
	void init();
};

#endif