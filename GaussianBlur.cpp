#include <GL/glew.h>
#include "GaussianBlur.h"
#include "Shader.h"
#include "RenderManager.h"
#include "Scene.h"
#include "GameManager.h"

#include <Windows.h>

GaussianBlur::GaussianBlur(bool bloom) : bloom(bloom) {}

void GaussianBlur::init()
{
	PostProcessing::init();
	pixelShader = initShader("gaussianblur");
}

void GaussianBlur::initFBO(int width, int height, int oldFbo)
{
	if (framebufferID != 0)
		return;

	glGenFramebuffersEXT(1, &framebufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);

	// initialize color textures
	glGenTexturesEXT(2, colorTextureID);
	for (int i = 0; i < 2; ++i) {
		glBindTexture(GL_TEXTURE_2D, colorTextureID[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, colorTextureID[i], 0);
	}

	// initialize depth renderbuffer
	glGenRenderbuffersEXT(1, &depthRenderBufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBufferID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32_ARB, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBufferID);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);
}

void GaussianBlur::drawToFBO(int oldFbo, int viewport[4])
{
	initFBO(viewport[2], viewport[3], 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);
	glViewport(0, 0, viewport[2], viewport[3]);

	if (viewport[0] == 0)
		GameManager::getInstance()->scene->draw(DrawMode::LeftEye);
	else
		GameManager::getInstance()->scene->draw(DrawMode::RightEye);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void GaussianBlur::draw()
{
	PostProcessing::draw();

	GLint oldFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	drawToFBO(oldFbo, viewport);

	glPushMatrix();

	glUseProgram(pixelShader);
	//GLint loc1 = glGetUniformLocation(pixelShader, "time");
	//glUniform1f(loc1, fpTime);
	glUniform1i(glGetUniformLocation(pixelShader, "bloom"), bloom);
	GLint loc = glGetUniformLocation(pixelShader, "texture");
	GLint loc2 = glGetUniformLocation(pixelShader, "firstPass");

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor4f(1, 1, 1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	//Render pass #1 - From Tex0 to Tex1
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);
	glDrawBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glViewport(0, 0, viewport[2], viewport[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, colorTextureID[0]);
	glUniform1f(loc, colorTextureID[0]);
	glUniform1i(loc2, true);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);     glVertex2i(-1, -1);
	glTexCoord2i(1, 0);     glVertex2i(1, -1);
	glTexCoord2i(1, 1);     glVertex2i(1, 1);
	glTexCoord2i(0, 1);     glVertex2i(-1, 1);
	glEnd();

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);

	//Render pass #2 - From Tex1 to Screen
	glBindTexture(GL_TEXTURE_2D, colorTextureID[1]);
	glUniform1f(loc, colorTextureID[1]);
	glUniform1i(loc2, false);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);     glVertex2i(-1, -1);
	glTexCoord2i(1, 0);     glVertex2i(1, -1);
	glTexCoord2i(1, 1);     glVertex2i(1, 1);
	glTexCoord2i(0, 1);     glVertex2i(-1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glPopMatrix();
}