#ifndef MENU_H
#define MENU_H

#include <glut\glut.h>
#include "Notes.h"

class Menu {

public:
	Menu();

	~Menu();

	void draw();
	void init();
	void drawButtons();
	void selectMenuItem();

	Notes* notes = new Notes();

};

#endif