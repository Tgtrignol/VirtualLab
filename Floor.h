#ifndef FLOOR_H
#define FLOOR_H

#include <vector>
#define FLOOR_WIDTH 240.0f
#define FLOOR_HEIGHT 240.0f

class Floor
{
public:
	void draw();

private:
	unsigned int textureID = 0;
	void initTexture();
};

#endif