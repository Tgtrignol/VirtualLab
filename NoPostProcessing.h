#ifndef NO_POST_PROCESSING_H
#define NO_POST_PROCESSING_H

#include "PostProcessing.h"

class NoPostProcessing : public PostProcessing{
public:
	GLuint pixelShader = 0;
	GLuint framebufferID = 0;
	GLuint colorTextureID;
	GLuint depthRenderBufferID = 0;

	NoPostProcessing();
	void init();
	void initFBO(int width, int height, int oldFbo);
	void draw();
	void drawToFBO(int oldFbo, int viewport[4]);
};

#endif