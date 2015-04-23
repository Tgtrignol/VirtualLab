#pragma once
#include "TerrainManager.h"

class FlatLandManager : public TerrainManager
{
public:
	FlatLandManager();
	~FlatLandManager();
	unsigned char * CreateRandomTerrain(unsigned int width, unsigned int height);
	void createVegetation(int amount, btDiscreteDynamicsWorld* world);
};
