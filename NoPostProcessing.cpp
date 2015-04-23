#include <GL/glew.h>
#include "NoPostProcessing.h"
#include "Shader.h"
#include "RenderManager.h"
#include "Level.h"
#include "GameManager.h"

#include <Windows.h>

NoPostProcessing::NoPostProcessing() {}

void NoPostProcessing::init()
{
	PostProcessing::init();
	pixelShader = initShader("noPostProcessing");
}

void NoPostProcessing::initFBO(int width, int height, int oldFbo)
{
	if (framebufferID != 0)
		return;

	glGenFramebuffersEXT(1, &framebufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);

	// initialize color textures
	glGenTexturesEXT(1, &colorTextureID);
	glBindTexture(GL_TEXTURE_2D, colorTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, colorTextureID, 0);

	// initialize depth renderbuffer
	glGenRenderbuffersEXT(1, &depthRenderBufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthRenderBufferID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT32_ARB, width, height);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthRenderBufferID);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);
}

void NoPostProcessing::drawToFBO(int oldFbo, int viewport[4])
{
	initFBO(viewport[2], viewport[3], 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebufferID);
	glViewport(0, 0, viewport[2], viewport[3]);

	if (viewport[0] == 0)
		GameManager::getInstance()->level->draw(DrawMode::LeftEye);
	else
		GameManager::getInstance()->level->draw(DrawMode::RightEye);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void NoPostProcessing::draw()
{
	PostProcessing::draw();

	GLint oldFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	drawToFBO(oldFbo, viewport);

	glPushMatrix();

	glUseProgram(pixelShader);

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

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFbo);

	//Render pass #2 - From Tex1 to Screen
	glBindTexture(GL_TEXTURE_2D, colorTextureID);
	GLint loc = glGetUniformLocation(pixelShader, "texture");
	glUniform1f(loc, colorTextureID);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);	glVertex2i(-1, -1);
	glTexCoord2i(1, 0);	glVertex2i(1, -1);
	glTexCoord2i(1, 1);	glVertex2i(1, 1);
	glTexCoord2i(0, 1);	glVertex2i(-1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
	glPopMatrix();
}