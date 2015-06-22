#ifndef GAUSSIAN_BLUR_H
#define GAUSSIAN_BLUR_H

#include "PostProcessing.h"
#include <glut\glut.h>

class GaussianBlur : public PostProcessing {

public:
	GLuint pixelShader = 0;
	GLuint framebufferID = 0;
	GLuint colorTextureID[2];
	GLuint depthRenderBufferID = 0;
	bool bloom = false;

	GaussianBlur(bool bloom);
	void init();
	void initFBO(int width, int height, int oldFbo);
	void draw();
	void drawToFBO(int oldFbo, int viewport[4]);
};

#endif