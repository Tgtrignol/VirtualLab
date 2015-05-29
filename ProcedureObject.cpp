#include <GL/glew.h>
#include "Control.h"
#include "ProcedureObject.h"
#include "Shader.h"
#include <btBulletCollisionCommon.h>
#include "ObjModel.h"
#include "Camera.h"
#include "GameManager.h"
#include "Scene.h"
#include "Hydra.h"

void ProcedureObject::init()
{
	origin = new btVector3(0.0, 0.0, 0.0); //TODO: Replace with a value read from file
	shaderID = initShader("procedureObject");
	btVector3 size(0.2, 0.45, 1.0);
	btScalar mass = 0.0;
	pObjModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\" + fileName, size, mass, *origin);
	pObjModel->rigidBody->setUserPointer(this);
}

void ProcedureObject::draw()
{
	glPushMatrix();
	glUseProgram(shaderID);

	btTransform trans;
	btScalar m[16];
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);

	//TODO: Remove hardcoded example
	glScalef(0.10, 0.10, 0.10);
	glScalef(1.0, 1.0, -1.0);

	GLint uniform = 0;

	//EXAMPLE: Turn this on and off.
	bool useColorInsteadOfTexture = true;//TODO: Replace true with a value read from file

	uniform = glGetUniformLocation(shaderID, "useTexture");
	glUniform1i(uniform, !useColorInsteadOfTexture); 

	if (useColorInsteadOfTexture)
	{
		uniform = glGetUniformLocation(shaderID, "color");
		glUniform4f(uniform, 1.0f, 0.0f, 0.0f, 0.5f); //TODO: Replace red with a value read form file
	}

	uniform = glGetUniformLocation(shaderID, "materialShininess");
	glUniform1f(uniform, 80.0f);

	uniform = glGetUniformLocation(shaderID, "materialSpecularColor");
	glUniform3f(uniform, 0.5f, 0.3f, 0.1f);

	uniform = glGetUniformLocation(shaderID, "light.position");
	glUniform3f(uniform, 0.0f, 1.0f, 0.0f); //TODO: Take a non-random place

	uniform = glGetUniformLocation(shaderID, "light.intensities");
	glUniform3f(uniform, 1.0f, 1.0f, 1.0f);

	uniform = glGetUniformLocation(shaderID, "light.attenuation");
	glUniform1f(uniform, 0.2f);

	uniform = glGetUniformLocation(shaderID, "light.ambientCoefficient");
	glUniform1f(uniform, 0.005f);

	uniform = glGetUniformLocation(shaderID, "cameraPosition");
	glUniform3f(uniform, fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate);

	pObjModel->draw(shaderID);

	glUseProgram(0);
	glPopMatrix();
}

void ProcedureObject::update()
{
	if (grabbed)
	{
		pObjModel->rigidBody->setGravity(btVector3(0, 0, 0));
		if (hydra == 0)
			origin = GameManager::getInstance()->scene->hydra->getLeftHydraCor();
		else if (hydra == 1)
			origin = GameManager::getInstance()->scene->hydra->getRightHydraCor();

		btTransform transform = pObjModel->rigidBody->getCenterOfMassTransform();
		transform.setOrigin(*origin);
		pObjModel->rigidBody->setCenterOfMassTransform(transform);
	}
}

void ProcedureObject::setGravity(btVector3* gravity)
{
	pObjModel->rigidBody->setGravity(btVector3(0, 0, 0));
}

void ProcedureObject::rotate(bool horizontal, int degrees)
{

}