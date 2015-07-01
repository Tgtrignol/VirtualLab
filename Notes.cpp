#include <stdlib.h>
#include <glut\glut.h>
#include "Notes.h"
#include "ProcedureManager.h"
#include "GameManager.h"
#include "Scene.h"
#include "ProcedureObject.h"
#include "Hydra.h"
#include "TextRepresentation.h"

Notes::Notes() {
	textRepresentation = new TextRepresentation();
}

void Notes::drawBoardTxt() {

	glPopMatrix();
	int itemNr = 0;

	for (int posY = 100; posY < 400; posY += 20) {
		itemNr++;
		std::string text = "Nr. " + std::to_string(itemNr) + " Column 1 en Column 2";
		textRepresentation->drawNotes(text.data(), text.size());
	}
	glPushMatrix();

}

void Notes::draw()
{

}

void Notes::init()
{

}