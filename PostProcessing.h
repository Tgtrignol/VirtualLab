#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include "RenderManager.h"

class PostProcessing {

public:
	PostProcessing();
	virtual void draw();
	virtual void init();

};

#endif