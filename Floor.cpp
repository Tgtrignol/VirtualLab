#include "Floor.h"
#pragma comment(lib, "glu32.lib") 
#include <GL/glew.h>
#include "stb_image.h"

void Floor::draw()
{
	initTexture();
	glColor3f(0.3f, 0.6f, 0.3f);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glScalef(4, 4, 4);
	glBegin(GL_QUADS);
	for (int i = (-FLOOR_WIDTH / 4.) / 2.; i < (FLOOR_WIDTH / 4.) / 2.; i++)
	{
		for (int j = (-FLOOR_HEIGHT / 4.) / 2; j < (FLOOR_HEIGHT / 4. / 2.); j++)
		{
			glNormal3f(0, 1, 0); glTexCoord2f(0, 0); glVertex3f(0.0f + i, 0, 1.0f + j);
			glNormal3f(0, 1, 0); glTexCoord2f(0, 1); glVertex3f(1.0f + i, 0, 1.0f + j);
			glNormal3f(0, 1, 0); glTexCoord2f(1, 0); glVertex3f(1.0f + i, 0, 0.0f + j);
			glNormal3f(0, 1, 0); glTexCoord2f(1, 1); glVertex3f(0.0f + i, 0, 0.0f + j);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void Floor::initTexture()
{
	if (textureID == 0)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width = 1024, height = 1024, bpp = 32;
		unsigned char* imgData = stbi_load("c:/VrCave/Development/VirtualLab/Data/grass4.png", &width, &height, &bpp, 4);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 
			4, 
			width, 
			height, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			imgData);

		//glTexImage2D(GL_TEXTURE_2D,
		//	0,					//Scene
		//	GL_RGBA,			//internal format
		//	width,				//width
		//	height,				//height
		//	0,					//border
		//	GL_RGBA,			//data format
		//	GL_UNSIGNED_BYTE,	//data type
		//	imgData);			//data

		stbi_image_free(imgData);

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}