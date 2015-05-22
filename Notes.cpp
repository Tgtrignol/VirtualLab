#include <stdlib.h>
#include <glut\glut.h>
#include <string>
#include "Notes.h"

using namespace std;

void Notes::drawNotes(const char *text, int length, int x, int y, int z) {

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glColor3f(0.0f, 0.0f, 0.0f);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3i(x, y, z);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void Notes::draw()
{
	for (int itemNr = 0; itemNr < 15; itemNr++) {
		string text = to_string(itemNr);
		drawNotes(text.data(), text.length(), 200, 200, 200);
	}

	std::string colum_1 = "colum 1";
	drawNotes(colum_1.data(), colum_1.length(), 250, 200, 200);

	std::string colum_2 = "colum 2";
	drawNotes(colum_2.data(), colum_2.length(), 350, 200, 200);

}

void Notes::init()
{

}