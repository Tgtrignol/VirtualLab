#include "GameManager.h"
#include "Scene.h"
#include "RenderManager.h"

#include <glm\matrix.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <VrLib\Application.h>
#include <GL\glew.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>

GameManager* GameManager::getInstance()
{
	static GameManager* pGameManager = new GameManager();
	return pGameManager;
}

void GameManager::init()
{
	scene = new Scene();
	renderManager = new RenderManager();
}

void GameManager::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	renderManager->draw();
}

void GameManager::preFrame(double frameTime, double totalTime)
{
	fpTime += frameTime;
	fpTimeUntillUpdate -= (frameTime / 1000.0f);

	if (fpTimeUntillUpdate <= 0) {
		scene->update(frameTime, totalTime);
		fpTimeUntillUpdate += GLOBAL_UPDATE_RATE;
	}
}

void GameManager::latePreFrame()
{

}

void GameManager::stop()
{
	delete scene;
	delete renderManager;
}