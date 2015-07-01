#include <iostream>
#include "Menu.h"
#include "Scene.h"
#include "GameManager.h"
#include "ProcedureManager.h"
#include "DSLReader.h"
#include "Notes.h"
#include "StaticMenu.h"

using namespace std;

Menu::Menu() {
	
	init();

	notes = new Notes();

	//staticMenu = new StaticMenu();
	//staticMenu->init();

}

Menu::~Menu() {

}

void Menu::selectMenuItem() {

	
	

}

void Menu::draw() {

	//staticMenu->draw();

	glPushMatrix();

	for (int i = 0; i < ProcedureManager::getInstance()->dslReader->menuItem.size(); i++) {

//	GameManager::getInstance()->scene->notes->drawNotes(ProcedureManager::getInstance()->dslReader->menuItem.data(),
	//	ProcedureManager::getInstance()->dslReader->menuItem.length(), 200.0f, 300.0f, 0.0f);
		

	}

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