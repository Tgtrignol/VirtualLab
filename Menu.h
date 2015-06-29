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
	void init();
	void drawButtons();
	void selectMenuItem();

	Notes* notes = new Notes();
	std::vector<std::string> procedureNames;

};

#endif