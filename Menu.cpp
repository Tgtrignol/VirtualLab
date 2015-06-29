#include <iostream>
#include "Menu.h"
#include "DSLReader.h"

Menu::Menu() {

}

Menu::~Menu() {

}

void Menu::drawButtons() {

	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(6.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glVertex3f(6.0f, 4.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void Menu::selectMenuItem() {



}

void Menu::draw() {

	//std::string exit = "Exit program";
	//notes->drawNotes(exit.data(), exit.length, 100.0f, 200.0f, 0.0f);



	drawButtons();

}

void Menu::init() {
	procedureFileLocations = DSLReader().readProcedureLocationFromFile();

	//Getting names from files
	for each (std::string file in procedureFileLocations)
	{
		std::string name = DSLReader().getProcedureName(file);
		if (name != "")
			procedureNames.push_back(name);
	}
}