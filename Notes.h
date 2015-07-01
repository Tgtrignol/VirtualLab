#ifndef NOTES_H
#define NOTES_H
#include <string>
#include <vector>
#include <stdlib.h>
#include <glut\glut.h>

struct ProcedureInformation;

class Notes {

public:
	Notes();
	void draw();
	void init();
	void drawNotes(const char*, int, int, int, int);
	void drawNotes(const char *text, int length, int x, int y, int z, GLfloat r, GLfloat g, GLfloat b);
	void drawStaticTxt(const char*, int);
	void drawBoardTxt();
	void drawButtonTxt();
	void drawList(std::vector<std::string>);
	void drawList(std::vector<std::string> list, int x, int y, int z);
	std::string buttonText;
	std::string errorText = "";

private:
	ProcedureInformation *m_pProcedureInformation;

};

#endif