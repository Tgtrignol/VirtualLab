#ifndef MAIN_H
#define MAIN_H

//Turn this on in simulator mode if sim.config is selected
#define SIMULATION_MODE

#define BULLET_DEBUG_DRAW
#ifdef _DEBUG

#pragma comment(lib, "Libs/Bullet3Common_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Collision_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Dynamics_vs2010_debug.lib")
#pragma comment(lib, "Libs/BulletCollision_vs2010_debug.lib")
#pragma comment(lib, "Libs/BulletDynamics_vs2010_debug.lib")
#pragma comment(lib, "Libs/Bullet3Geometry_vs2010_debug.lib")
#pragma comment(lib, "Libs/LinearMath_vs2010_debug.lib")
#pragma comment(lib, "Libs/libnoise.lib")
#else
#pragma comment(lib, "Libs/Bullet3Common_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Collision_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Dynamics_vs2010.lib")
#pragma comment(lib, "Libs/BulletCollision_vs2010.lib")
#pragma comment(lib, "Libs/BulletDynamics_vs2010.lib")
#pragma comment(lib, "Libs/Bullet3Geometry_vs2010.lib")
#pragma comment(lib, "Libs/LinearMath_vs2010.lib")
#pragma comment(lib, "Libs/libnoise.lib")
#endif

#include <Windows.h>
#include <GL\glew.h>
#include <btBulletDynamicsCommon.h>

extern btBroadphaseInterface* broadphase;
extern btDefaultCollisionConfiguration* collisionConfiguration;
extern btCollisionDispatcher* dispatcher;
extern btSequentialImpulseConstraintSolver* solver;
extern btDiscreteDynamicsWorld* world;

void main(int argc, char* argv[]);

#endif