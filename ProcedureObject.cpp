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

std::string originAnchorToString(OriginAnchor originAnchor)
{
	switch (originAnchor)
	{
	case OriginAnchor::Room:
		return "Room";
	case OriginAnchor::Table:
		return "Table";
	case OriginAnchor::Error:
		return "Error";
	}
}

OriginAnchor stringToOriginAnchor(std::string str)
{
	if (str == "Room")
		return OriginAnchor::Room;
	else if (str == "Table")
		return OriginAnchor::Table;
	else
		return OriginAnchor::Error;
}

void ProcedureObject::applyOriginAnchorTranslation()
{
	if (originAnchor == OriginAnchor::Room) {
		//Empty on purpose
	}
	else if (originAnchor == OriginAnchor::Table) {
		btVector3 vec = *origin;
		delete origin;
		origin = new btVector3(vec.x() + -1, vec.y() + 0.9, vec.z() + -0.15);
	}
}

void ProcedureObject::init()
{
	shaderID = initShader("procedureObject");

	applyOriginAnchorTranslation();
	const btVector3 &refScale = *scale;
	pObjModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\" + fileName, 0.0, *origin, refScale);
	pObjModel->rigidBody->setUserPointer(this);

	//Rigidbody transform
	btTransform trans;
	trans = pObjModel->rigidBody->getCenterOfMassTransform();
	trans.setRotation(btQuaternion(TO_RADIANS(rotation->y()), TO_RADIANS(rotation->x()), TO_RADIANS(rotation->z())));
	pObjModel->rigidBody->setCenterOfMassTransform(trans);

	//Object transform
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion(TO_RADIANS(rotation->y()), TO_RADIANS(rotation->x()), TO_RADIANS(rotation->z())));
	pObjModel->rigidBody->getMotionState()->setWorldTransform(trans);
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

	//glRotatef(rotation->x(), 1, 0, 0);
	//glRotatef(rotation->y(), 0, 1, 0);//TODO: check axis
	//glRotatef(rotation->z(), 0, 0, 1);
	glScalef(scale->x(), scale->y(), scale->z());

	GLint uniform = 0;

	uniform = glGetUniformLocation(shaderID, "useTexture");
	glUniform1i(uniform, !useColorInsteadOfTexture);

	if (useColorInsteadOfTexture)
	{
		uniform = glGetUniformLocation(shaderID, "color");
		glUniform4f(uniform, color->x(), color->y(), color->z(), color->w());
	}

	uniform = glGetUniformLocation(shaderID, "useWater");
	glUniform1i(uniform, useWaterOverlay);

	if (useWaterOverlay)
	{
		uniform = glGetUniformLocation(shaderID, "waterVertMin");
		glUniform3f(uniform, waterDirectionMin->x(), waterDirectionMin->y(), waterDirectionMin->z());

		uniform = glGetUniformLocation(shaderID, "waterVertMax");
		glUniform3f(uniform, waterDirectionMax->x(), waterDirectionMax->y(), waterDirectionMax->z());
	}

	uniform = glGetUniformLocation(shaderID, "materialShininess");
	glUniform1f(uniform, 10.0f);

	uniform = glGetUniformLocation(shaderID, "materialSpecularColor");
	glUniform3f(uniform, 0.5f, 0.3f, 0.1f);

	uniform = glGetUniformLocation(shaderID, "light.position");
	glUniform3f(uniform, 0.82f, 1.85f, 1.45f);

	uniform = glGetUniformLocation(shaderID, "light.intensities");
	glUniform3f(uniform, 1.0f, 1.0f, 1.0f);

	uniform = glGetUniformLocation(shaderID, "light.attenuation");
	glUniform1f(uniform, 0.2f);

	uniform = glGetUniformLocation(shaderID, "light.ambientCoefficient");
	glUniform1f(uniform, 0.5f);

	uniform = glGetUniformLocation(shaderID, "cameraPosition");
	glUniform3f(uniform, fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate);

	GLfloat modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	uniform = glGetUniformLocation(shaderID, "modelView");
	glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, modelViewMatrix);

	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	uniform = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, projectionMatrix);

	pObjModel->draw(shaderID);

	glUseProgram(0);
	glPopMatrix();
}

void ProcedureObject::update()
{
	if (!initRigidbodies)
	{
		GameManager::getInstance()->scene->world->addRigidBody(pObjModel->rigidBody);
		initRigidbodies = true;
	}

	if (grabbed)
	{
		//pObjModel->rigidBody->setGravity(btVector3(0, 0, 0));
		if (LeftRight == "Left")
			origin = GameManager::getInstance()->scene->hydra->getLeftHydraCor();
		else if (LeftRight == "Right")
			origin = GameManager::getInstance()->scene->hydra->getRightHydraCor();

		//Rigidbody transform
		pObjModel->rigidBody->setActivationState(1);
		btTransform transform = pObjModel->rigidBody->getCenterOfMassTransform();
		transform.setOrigin(*origin);
		pObjModel->rigidBody->setCenterOfMassTransform(transform);

		//Object transform
		pObjModel->rigidBody->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(*origin);
		pObjModel->rigidBody->getMotionState()->setWorldTransform(transform);
	}
}

void ProcedureObject::rotate(std::string direction, int degrees)
{
	//Rigidbody transform
	btTransform transOb;
	btTransform transRig;
	transRig = pObjModel->rigidBody->getCenterOfMassTransform();
	pObjModel->rigidBody->getMotionState()->getWorldTransform(transOb);

	if (direction == "X")
	{
		rotation->setX(rotation->x() + degrees);
	}
	else if (direction == "Y")
	{
		rotation->setY(rotation->y() + degrees);
	}
	else if (direction == "Z")
	{
		rotation->setZ(rotation->z() + degrees);
	}

	transRig.setRotation(btQuaternion(TO_RADIANS(rotation->y()), TO_RADIANS(rotation->x()), TO_RADIANS(rotation->z())));
	transOb.setRotation(btQuaternion(TO_RADIANS(rotation->y()), TO_RADIANS(rotation->x()), TO_RADIANS(rotation->z())));

	pObjModel->rigidBody->setCenterOfMassTransform(transRig);
	pObjModel->rigidBody->getMotionState()->setWorldTransform(transOb);
}