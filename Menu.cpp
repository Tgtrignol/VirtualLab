#include <iostream>
#include "Menu.h"
#include "Scene.h"
#include "GameManager.h"
#include "ProcedureManager.h"
#include "DSLReader.h"
#include "Notes.h"

using namespace std;

Menu::Menu() {

}

Menu::~Menu() {

}

void Menu::drawButtons(int x, int y, int z) {

	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glTranslatef(x, y, z);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(6.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 4.0f, 0.0f);
		glVertex3f(6.0f, 4.0f, 0.0f);
	glEnd();
	glPopMatrix();
}

void Menu::selectMenuItem() {


	//GameManager::getInstance()->scene->procedureManager->procedureFile = chosenFile;

}

void Menu::draw() {

	glPushMatrix();

	//for (int i = 0; i < ProcedureManager::getInstance()->dslReader->menuItem.size(); i++) {

	GameManager::getInstance()->scene->notes->drawNotes(ProcedureManager::getInstance()->dslReader->menuItem.data(),
		ProcedureManager::getInstance()->dslReader->menuItem.length(), 200.0f, 300.0f, 0.0f);
		drawButtons(200.0f, 300.0f, 0.0f);

	//}

	//std::string text = "Exit program";
	//GameManager::getInstance()->scene->notes->drawNotes(text.data(), text.length(), 470.0f, 300.0f, 0.0f);

	glPopMatrix();

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