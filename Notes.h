#ifndef NOTES_H
#define NOTES_H
#include <string>
#include <vector>

struct ProcedureInformation;

class Notes {

public:
	Notes();
	void draw();
	void init();
	void drawNotes(const char*, int, int, int, int);
	void drawBoardTxt();
	void drawButtonTxt();
	void drawList(std::vector<std::string>);
	std::string buttonText;
	std::string errorText = "";

private:
	ProcedureInformation *m_pProcedureInformation;

};

#endif