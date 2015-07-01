#ifndef MENU_H
#define MENU_H

#include <glut\glut.h>
#include <vector>
#include "Notes.h"

class StaticMenu;

class Menu {

private:
	std::vector<std::string> procedureFileLocations;
public:
	Menu();

	~Menu();

	void draw();
	void init();
	void drawButtons(int, int, int);
	void selectMenuItem();

	Notes* notes;
	//StaticMenu* staticMenu;
	std::vector<std::string> procedureNames;

};

#endif