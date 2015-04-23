#pragma once
#include "TerrainManager.h"

class MountainManager : public TerrainManager
{
public:
	MountainManager();
	~MountainManager();
	unsigned char * CreateRandomTerrain(unsigned int width, unsigned int height);
	void createVegetation(int amount, btDiscreteDynamicsWorld* world);
};
