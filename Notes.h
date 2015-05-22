#ifndef NOTES_H
#define NOTES_H

struct ProcedureInformation;

class Notes {

public:
	void draw();
	void init();
	void drawNotes(const char*, int, int, int, int);

private:
	ProcedureInformation *m_pProcedureInformation;

};

#endif