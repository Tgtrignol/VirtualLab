#include <stdlib.h>
#include <glut\glut.h>
#include "TextRepresentation.h"

TextRepresentation::TextRepresentation() {

}

TextRepresentation::~TextRepresentation() {

}

void TextRepresentation::drawNotes(const char *text, int length) {

	glPushMatrix();

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
	}

	glPopMatrix();

}

void TextRepresentation::drawText(const char *text, int length, int x, int y, int z) {

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();
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
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void TextRepresentation::draw() {

}

void TextRepresentation::init() {

}