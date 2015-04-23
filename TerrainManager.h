#pragma once
#include <vector>
#include "StaticEnvironment.h"
#include <btBulletDynamicsCommon.h>

class TerrainManager
{
public:
	std::vector<StaticEnvironment *> environmentObjects;

	virtual unsigned char * CreateRandomTerrain(unsigned int width, unsigned int height);
	virtual void createVegetation(int amount, btDiscreteDynamicsWorld* world);
};
