#ifndef NOTES_H
#define NOTES_H
#include <string>

struct ProcedureInformation;

class Notes {

public:
	Notes();
	void draw();
	void init();
	void drawNotes(const char*, int, int, int, int);
	void drawBoardTxt();
	void drawButtonTxt();
	std::string buttonText;
	std::string errorText = "";

private:
	ProcedureInformation *m_pProcedureInformation;

};

#endif