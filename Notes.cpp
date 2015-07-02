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

/*
void Notes::drawLabel(int x, int y, std::string string) {

		glTranslatef(2, 0, 0);
		glRotatef(-45, 0, 1, 0);
		glTranslatef(0, -1, -1);
		partLabel.text = string;
		//glTranslatef(x, y, 0);
		partLabel.x = x;
		partLabel.y = y;
		partLabel.render(2.0f);

}
*/

void Notes::drawBoardTxt() {

	/*
	glPopMatrix();
	int itemNr = 0;

	for (int posY = 100; posY < 400; posY += 20) {
		itemNr++;
		std::string text = "Nr. " + std::to_string(itemNr) + " Column 1 en Column 2";
		textRepresentation->drawNotes(text.data(), text.size());
	}
	glPushMatrix();
	*/
}

void Notes::draw()
{

}

void Notes::init()
{

}