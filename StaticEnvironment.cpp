#include "StaticEnvironment.h"
#include <GL/glew.h>
#include "ObjModel.h"
#include "Shader.h"
#include "GameManager.h"
#include "Level.h"
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

//////////////////////////////
//EXAMPLE ONLY, REMOVE LATER//
//////////////////////////////

void StaticEnvironment::init()
{
	// test:
	//shaderID = initShader("simple");
	//btVector3 size(0.2, 0.45, 1.0);
	//btScalar mass = 10.0;
	//pObjModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\wolf3\\Wolf.obj", size, mass, btVector3(0, 0, 0));
}

void StaticEnvironment::draw()
{
	//glUseProgram(shaderID);
	//GLint loc = glGetUniformLocation(shaderID, "glow");
	//btTransform trans;
	//btScalar m[16];

	////Normal draw
	//glDepthMask(GL_TRUE);
	//glUniform1i(loc, false);

	//glPushMatrix();
	//pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	//trans.getOpenGLMatrix(m);
	//glMultMatrixf(m);
	//glScalef(0.5, 0.5, 0.5);

	//glTranslatef(0, -1, 0);
	//pObjModel->draw(shaderID);

	//glPopMatrix();

	//glUseProgram(0);
}
