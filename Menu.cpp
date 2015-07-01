#include <iostream>
#include "Menu.h"
#include "Scene.h"
#include "GameManager.h"
#include "ProcedureManager.h"
#include "DSLReader.h"
#include "Notes.h"
#include "StaticMenu.h"
#include "TextRepresentation.h"

using namespace std;

Menu::Menu() {

	textRepresentation = new TextRepresentation();

}

Menu::~Menu() {

}

void Menu::draw() {

	if (errorText != "")
		textRepresentation->drawText(errorText.data(), errorText.length(), 200, 340, 0);

}

void Menu::drawList(std::vector<std::string> list)
{
	
	for (int i = 0; i < list.size(); i++)
	{
		textRepresentation->drawText(list[i].data(), list[i].length(), 200, 300 + (i * 10), 0);
	}

}

void Menu::drawCursor(int menuItem) {

	string cursor = ">";
	textRepresentation->drawText(cursor.data(), cursor.length(), 190, 300 + (menuItem * 10), 0);

	string txt = "Procedure: " + std::to_string(menuItem);
	textRepresentation->drawText(txt.data(), txt.length(), 200, 370, 0);

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
	numberOfProcedures = procedureFileLocations.size();
}