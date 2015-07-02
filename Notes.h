#ifndef NOTES_H
#define NOTES_H
//#include <CaveLib\Components\Label.h>
//#include <CaveLib\font.h>
#include <stdlib.h>
#include <glut\glut.h>
#include <string>
#include <vector>

struct ProcedureInformation;

class TextRepresentation;

class Notes {

public:
	Notes();
	void draw();
	void init();
	//void drawLabel(int, int, std::string string);
	void drawBoardTxt();

	//Label partLabel = Label("Nothing selected!");
	//cFont* font = new cFont("Tahoma");

private:
	ProcedureInformation *m_pProcedureInformation;
	TextRepresentation *textRepresentation;

};

#endif