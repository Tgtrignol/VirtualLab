#ifndef HYDRA_H
#define HYDRA_H

#define SWORD_Y_OFFSET -1.0f //Used in order to align the hitbox and the drawn model.
#define TO_RADIANS(X) float(X / 180.0f * 3.14159265358979323846f)

#include <glm\glm.hpp>
#include <VrLib/Device.h>
#include <btBulletDynamicsCommon.h>

class ObjModel;

class Hydra
{
public:
	void Hydra::draw(float InitialModelView[16]);
	void update();
	void init();
	bool initRigidbodies = false;
	ObjModel *leftModel;
	ObjModel *rightModel;
	btVector3* getRightHydraCor();
	btVector3* getLeftHydraCor();
	std::string checkButtons();

private:
	unsigned int shaderID;
	PositionalDevice hydraRightPosition;
	PositionalDevice hydraLeftPosition;
	TwoDimensionDevice hydraRightJoystick;
	TwoDimensionDevice hydraLeftJoystick;
	AnalogDevice hydraLeftTrigger;
	DigitalDevice hydraLeftBumper;

	DigitalDevice hydraLeftOne;
	DigitalDevice hydraRightOne;
	DigitalDevice hydraLeftTwo;
	DigitalDevice hydraRightTwo;
	DigitalDevice hydraLeftThree;
	DigitalDevice hydraRightThree;
	DigitalDevice hydraLeftFour;
	DigitalDevice hydraRightFour;

	glm::vec4 hydraRightPositionVector;
	glm::vec4 hydraRightOrientation;
	glm::vec4 hydraLeftPositionVector;
	glm::vec4 hydraLeftOrientation;
	bool hydraEnabled = false;
	bool hydraPressed = false;

	btVector3 hydraRightVector = btVector3(10, 0, 0);
	btVector3 hydraLeftVector = btVector3(-10, 0, 0);

	glm::mat4 getWorldMatrixFromHydra(glm::mat4 old);
	void initHydraModels();
};

#endif