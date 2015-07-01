#ifndef MENU_H
#define MENU_H

#include <glut\glut.h>
#include <vector>
#include "Notes.h"

class Menu {

private:
	std::vector<std::string> procedureFileLocations;
public:
	Menu();

	~Menu();

	void draw();
	void drawCursor(int);
	void drawList(std::vector<std::string> list);
	void init();

	TextRepresentation* textRepresentation;
	std::vector<std::string> procedureNames;

	int numberOfProcedures = 0;

};

#endif