#ifndef MY_APPLICATION_H
#define MY_APPLICATION_H

#define GLOBAL_UPDATE_RATE 1.0f / 30.0f

#include <VrLib\Application.h>

class RenderManager;
class Scene;

class GameManager : public Application
{
public:
	RenderManager *renderManager;
	Scene *scene;
	float fpTime = 0;
	float fpTimeUntillUpdate = 0;

	static GameManager* getInstance();
	void init();
	void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
	void preFrame(double frameTime, double totalTime);
	
	void latePreFrame();
	
	void stop();
};

#endif