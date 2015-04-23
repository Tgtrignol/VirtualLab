#include <GL/glew.h>
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Level.h"
#include "PostProcessing.h"
#include "GaussianBlur.h"
#include "NoPostProcessing.h"

RenderManager::RenderManager()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_CULL_FACE); //Ouchta set normals for this to work.
	//glCullFace(GL_BACK);
	glClearColor(0, 0, 0, 0);
	postProcessor = new NoPostProcessing();
	postProcessor->init();
}

RenderManager::~RenderManager()
{
	delete postProcessor;
}

void RenderManager::draw()
{
	initLighting();
	postProcessor->draw();
}

void RenderManager::initLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	const float amb = 0.5;
	const float LightAmbient[4] = { amb, amb, amb, 1.0f };
	const float LightDiffuse[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	const float whiteSpecularLight[] = { 1.0, 1.0, 1.0 };
	const GLfloat shiny = 60.0;
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glLightfv(GL_LIGHT0, GL_SHININESS, &shiny);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
}
