#include "MountainManager.h"
#include <noise\noise.h>
#include "noiseutils\noiseutils.h"
#include <iostream>
#include <ctime>
#include <btBulletDynamicsCommon.h>

using namespace noise;
using namespace std;

MountainManager::MountainManager()
{

}
MountainManager::~MountainManager()
{

}

unsigned char * MountainManager::CreateRandomTerrain(unsigned int width, unsigned int height)
{
	srand(time(NULL));
	bool success = false;
	// preparing
	module::RidgedMulti mountainTerrain;
	mountainTerrain.SetLacunarity(3.0);
	//mountainTerrain.SetSeed(rand());

	module::Billow baseFlatTerrain;
	baseFlatTerrain.SetFrequency(4.0);

	module::ScaleBias flatTerrain;
	flatTerrain.SetSourceModule(0, baseFlatTerrain);
	flatTerrain.SetScale(0.125);
	flatTerrain.SetBias(-0.35);

	module::Perlin terrainType;
	terrainType.SetFrequency(0.5);
	terrainType.SetPersistence(1.0);

	module::Select finalTerrain;
	finalTerrain.SetSourceModule(0, flatTerrain);
	finalTerrain.SetSourceModule(1, mountainTerrain);
	finalTerrain.SetControlModule(terrainType);
	finalTerrain.SetBounds(0.0, 1000.0);
	finalTerrain.SetEdgeFalloff(0.1);

	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(finalTerrain);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(width, height);
	heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// creating the image
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.Render();

	// rewrite the image to byte array thingy that can be used by the Terrain
	const int size = image.GetWidth() * image.GetHeight();
	unsigned char * map = new unsigned char[size];

	for (int y = 0; y < image.GetHeight(); ++y)
	{
		for (int x = 0; x < image.GetWidth(); ++x)
		{
			map[y * image.GetWidth() + x] = image.GetValue(x, y).red;
		}
	}
	return map;
}

void MountainManager::createVegetation(int amount, btDiscreteDynamicsWorld* world)
{
	// TODO: implement this with models and spreading for mountain terrain.
}
