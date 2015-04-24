#ifndef NOTES_H
#define NOTES_H

class ProcedureInformation;

class Notes{
public:
	void draw();
	void init();
private:
	ProcedureInformation *m_pProcedureInformation;
};

#endif