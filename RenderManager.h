#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <VrLib\Application.h>
#include "GameManager.h"

class PostProcessing;

class RenderManager{
public:
	PostProcessing *postProcessor;

	RenderManager();
	~RenderManager();
	void draw();
	void initLighting();
};

#endif