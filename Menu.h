#ifndef MENU_H
#define MENU_H

#include <glut\glut.h>

class Menu {

public:
	Menu();

	~Menu();

	void draw();
	void init();
	void drawButtons(int, int, int);
	void selectMenuItem();

};

#endif