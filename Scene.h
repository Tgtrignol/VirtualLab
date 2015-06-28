#ifndef Scene_H
#define Scene_H

#include <vector>
#include <VrLib\Device.h>

class Floor;
class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class PositionalDevice;
class btRigidBody;
class Hydra;
class CameraCharacter;
class StaticLabEnvironment;
class ProcedureManager;
class Notes;
class HUD;
class Menu;

enum CollisionState
{
	NO_COLLISION,
	COLLISION
};

struct CollisionInformation
{
	CollisionInformation(CollisionState state, long collisionTime) : m_state(state), m_collisionTime(collisionTime) { }
	CollisionInformation(){}
	CollisionState m_state = CollisionState::NO_COLLISION;
	long m_collisionTime = LONG_MAX;
	bool m_informationUsed = false;
};

extern CollisionInformation leftHydraCollisionInformation;
extern CollisionInformation rightHydraCollisionInformation;

enum DrawMode
{
	RightEye,
	LeftEye,
	Simulation
};

class Scene{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	//oculus
	PositionalDevice *head;
	//End Oculus

	//Keyboard
	DigitalDevice *WKey;
	DigitalDevice *AKey;
	DigitalDevice *SKey;
	DigitalDevice *DKey;
	DigitalDevice *QKey;
	DigitalDevice *EKey;
	DigitalDevice *UpKey;
	DigitalDevice *DownKey;
	DigitalDevice *LeftKey;
	DigitalDevice *RightKey;
	//End Keyboard

public:
	Floor *f;
	Hydra *hydra;
	StaticLabEnvironment *lab;
	btDiscreteDynamicsWorld *world;
	ProcedureManager *procedureManager;
	Notes *notes;
	HUD *hud;
	Menu *menu;

	Scene();
	~Scene();
	void draw(DrawMode drawMode);
	void update();
	void update(double frameTime, double totalTime);
	unsigned int textureID = 0;
};

#endif