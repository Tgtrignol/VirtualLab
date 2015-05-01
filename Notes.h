#ifndef NOTES_H
#define NOTES_H

struct ProcedureInformation;

class Notes{
public:
	void draw();
	void init();
private:
	ProcedureInformation *m_pProcedureInformation;
};

#endif