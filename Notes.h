#ifndef NOTES_H
#define NOTES_H
#include <string>
#include <vector>

struct ProcedureInformation;

class TextRepresentation;

class Notes {

public:
	Notes();
	void draw();
	void init();
	void drawBoardTxt();

private:
	ProcedureInformation *m_pProcedureInformation;
	TextRepresentation *textRepresentation;

};

#endif