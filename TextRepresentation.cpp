#include <stdlib.h>
#include <glut\glut.h>
#include "TextRepresentation.h"

TextRepresentation::TextRepresentation() {

}

TextRepresentation::~TextRepresentation() {

}

void TextRepresentation::drawNotes(const char *text, int length, float originX, float originY, float originZ, float yOffset) {

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(originX, originY + yOffset, originZ);
	glScalef(0.01, 0.0125, 0.01);

	for (int i = 0; i < length; i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
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

void TextRepresentation::drawText(const char *text, int length, int x, int y, int z, GLfloat r, GLfloat g, GLfloat b) {

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glColor3f(r, g, b);

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

void TextRepresentation::drawList(std::vector<std::string> list)
{

	for (int i = 0; i < list.size(); i++)
	{
		drawText(list[i].data(), list[i].length(), 200, 300 + (i * 10), 0);
	}
}

void TextRepresentation::drawList(std::vector<std::string> list, int x, int y, int z)
{
	for (int i = 0; i < list.size(); i++)
	{
		drawText(list[i].data(), list[i].length(), x, y - (i * 10), z, 0.0f, 0.0f, 0.0f);
	}
}