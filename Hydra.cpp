#include "Hydra.h"
#include "Camera.h"
#include <VrLib\Application.h>
#include <VrLib\Device.h>
#include <GL/glew.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "ObjModel.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include "GameManager.h"
#include "Level.h"
#include <glm/gtc/matrix_transform.hpp>

void Hydra::init()
{
	shaderID = initShader("lightingShader");

	hydraRightPosition.init("RightNunchuck");
	hydraLeftPosition.init("LeftNunchuck");
	hydraRightJoystick.init("RightJoystick");
	hydraLeftJoystick.init("LeftJoystick");

	hydraLeftTrigger.init("LeftTrigger");
	hydraLeftBumper.init("LeftBumper");

	initHydraModels();
}

void Hydra::draw(float InitialModelView[16])
{
	if (!hydraEnabled)
	{
		return;
	}

	glPushMatrix();
	btScalar m[16];
	btTransform trans;

	glUseProgram(shaderID);
	//set the shader uniforms
	GLint uniform = 0;
	GLfloat projMatrix[16];
	GLfloat modelViewMatrix[16];

	uniform = glGetUniformLocation(shaderID, "materialShininess");
	glUniform1f(uniform, 80.0f);

	uniform = glGetUniformLocation(shaderID, "materialSpecularColor");
	glUniform3f(uniform, 0.5f, 0.3f, 0.1f); // dunno, random something something

	uniform = glGetUniformLocation(shaderID, "light.position");
	glUniform3f(uniform, 0.0f, 1.0f, 0.0f); // somewhere

	uniform = glGetUniformLocation(shaderID, "light.intensities");
	glUniform3f(uniform, 1.0f, 1.0f, 1.0f); // white

	uniform = glGetUniformLocation(shaderID, "light.attenuation");
	glUniform1f(uniform, 0.2f);

	uniform = glGetUniformLocation(shaderID, "light.ambientCoefficient");
	glUniform1f(uniform, 0.005f);

	uniform = glGetUniformLocation(shaderID, "cameraPosition");
	glUniform3f(uniform, fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate);

	//NOTE: This part is currently very dependend of the direction the hydra-globe is pointing, be sure to make the razer logo point towards you.
	//Hydra draw

	//Right Hydra
	{
		glPushMatrix();
		glLoadMatrixf(InitialModelView);
		rightModel->rigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(m);
		glTranslatef(hydraRightPositionVector[0], hydraRightPositionVector[1] - 1.0f, -2.0f + hydraRightPositionVector[2]);

		glm::mat4 old = hydraRightPosition.getData();

		glMultMatrixf(glm::value_ptr(getWorldMatrixFromHydra(old)));
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

		glTranslatef(0.0f, SWORD_Y_OFFSET, 0.0f);

		glScalef(0.0018f, 0.0018f, 0.0018f);


		uniform = glGetUniformLocation(shaderID, "projection");
		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, projMatrix);

		uniform = glGetUniformLocation(shaderID, "modelView");
		glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, modelViewMatrix);
		rightModel->draw(shaderID);

		glPopMatrix();
	}

	//Left Hydra
	{
		glPushMatrix();
		glLoadMatrixf(InitialModelView);
		leftModel->rigidBody->getMotionState()->getWorldTransform(trans);
		trans.getOpenGLMatrix(m);
		glTranslatef(hydraLeftPositionVector[0], hydraLeftPositionVector[1] - 1.0f, -2.0f + hydraLeftPositionVector[2]);

		glm::mat4 old = hydraLeftPosition.getData();

		glMultMatrixf(glm::value_ptr(getWorldMatrixFromHydra(old)));
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

		glTranslatef(0.0f, SWORD_Y_OFFSET, 0.0f);

		glScalef(0.0018f, 0.0018f, 0.0018f);


		uniform = glGetUniformLocation(shaderID, "projection");
		glGetFloatv(GL_PROJECTION_MATRIX, projMatrix);
		glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, projMatrix);

		uniform = glGetUniformLocation(shaderID, "modelView");
		glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
		glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, modelViewMatrix);
		leftModel->draw(shaderID);

		glPopMatrix();
	}
	glUseProgram(0);
	//End of hydra draw

	glPopMatrix();
}

void Hydra::update()
{
	if (hydraRightJoystick.isInitialized())
	{
		if ((!initRigidbodies) && hydraEnabled)
		{
			GameManager::getInstance()->level->world->addRigidBody(rightModel->rigidBody);
			GameManager::getInstance()->level->world->addRigidBody(leftModel->rigidBody);
			initRigidbodies = true;
		}

		glm::mat4 hydraMatrix = hydraRightPosition.getData();
		hydraRightPositionVector = hydraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // The origin of the razer hydra.
		hydraRightOrientation = glm::normalize((hydraMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)) - hydraRightPositionVector); // Where the razer hydra point to.

		btVector3 btFrom(hydraRightPositionVector.x, hydraRightPositionVector.y, hydraRightPositionVector.z);
		btVector3 btTo(hydraRightOrientation.x, hydraRightOrientation.y, hydraRightOrientation.z);
		btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);

		GameManager::getInstance()->level->world->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

		if (res.hasHit()){
			float yCoord = res.m_hitPointWorld.y();
			printf("%f",yCoord);
		}

		// Rotation of the camera. 
		glm::vec2 joystickData = hydraRightJoystick.getData();
		if (!((joystickData[0] <= -900) && (joystickData[1] <= -900)))
		{
			hydraEnabled = true;
			if (joystickData[0] - 0.1f > 0.0f)
				RotateRight();
			else if (joystickData[0] + 0.1f < 0.0f)
				RotateLeft();

			if (joystickData[1] - 0.1f > 0.0f)
				RotateUp();
			else if (joystickData[1] + 0.1f < 0.0f)
				RotateDown();
		}

		//Activate Right
		rightModel->rigidBody->setActivationState(1);

		btTransform transform = rightModel->rigidBody->getCenterOfMassTransform();
		float cameraYAngle = TO_RADIANS(-fpXAngle);
		float cameraXAngle = TO_RADIANS(-fpYAngle);

		glm::mat4 tra;
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		tra = glm::translate(tra, glm::vec3(hydraRightPositionVector.x * 2., hydraRightPositionVector.y * 2.0f - 1.0f, hydraRightPositionVector.z * 2.0f - 2.0f));
		glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		transform.setOrigin(btVector3(tran.x, tran.y, tran.z));

		auto v = glm::quat_cast((hydraRightPosition.getData()));
		btQuaternion quat(v.x, v.y, v.z, v.w);
		transform.setRotation(quat);

		rightModel->rigidBody->setCenterOfMassTransform(transform);

		btTransform tr = rightModel->rigidBody->getWorldTransform();
		tr.setRotation(btQuaternion(-cameraYAngle, -cameraXAngle, 0.0f) * tr.getRotation());
		rightModel->rigidBody->setWorldTransform(tr);
	}
	if (hydraLeftJoystick.isInitialized())
	{
		glm::mat4 hydraMatrix = hydraLeftPosition.getData();
		hydraLeftPositionVector = hydraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		hydraLeftOrientation = glm::normalize((hydraMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)) - hydraLeftPositionVector);

		glm::vec2 joystickData = hydraLeftJoystick.getData();
		if (!((joystickData[0] <= -900) && (joystickData[1] <= -900)))
		{
			hydraEnabled = true;
			if (joystickData[0] - 0.1f > 0.0f)
				MoveRight();
			else if (joystickData[0] + 0.1f < 0.0f)
				MoveLeft();

			if (joystickData[1] - 0.1f > 0.0f)
				MoveForward();
			else if (joystickData[1] + 0.1f < 0.0f)
				MoveBackward();
		}

		//Activate Left
		leftModel->rigidBody->setActivationState(1);

		btTransform transform = leftModel->rigidBody->getCenterOfMassTransform();
		float cameraYAngle = TO_RADIANS(-fpXAngle);
		float cameraXAngle = TO_RADIANS(-fpYAngle);

		glm::mat4 tra;
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
		tra = glm::translate(tra, glm::vec3(hydraLeftPositionVector.x * 2.0f, hydraLeftPositionVector.y * 2.0f - 1.0f, hydraLeftPositionVector.z * 2.0f - 2.0f));
		glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		transform.setOrigin(btVector3(tran.x, tran.y, tran.z));

		auto v = glm::quat_cast((hydraLeftPosition.getData()));
		btQuaternion quat(v.x, v.y, v.z, v.w);
		transform.setRotation(quat);

		leftModel->rigidBody->setCenterOfMassTransform(transform);

		btTransform tr = leftModel->rigidBody->getWorldTransform();
		tr.setRotation(btQuaternion(-cameraYAngle, -cameraXAngle, 0.0f) * tr.getRotation());
		leftModel->rigidBody->setWorldTransform(tr);
	}
	if (hydraLeftBumper.isInitialized())
	{
		if (hydraLeftBumper.getData() == DigitalState::ON || hydraLeftBumper.getData() == DigitalState::TOGGLE_ON)
			MoveUpward();
	}
	if (hydraLeftTrigger.isInitialized())
	{
		if (hydraLeftTrigger.getData() - 0.1f > 0.0f)
			MoveDownward();
	}
}

void Hydra::initHydraModels()
{
	btVector3 size = btVector3(0.1f, 0.1f, 0.85f);
	btScalar mass = 0;
	rightModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\Sword01\\rusword.obj", size, mass, btVector3(0, -100, 0));
	leftModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\Sword02\\rusword.obj", size, mass, btVector3(0, -100, 0));
}

glm::mat4 Hydra::getWorldMatrixFromHydra(glm::mat4 old)
{
	glm::mat4 m(old[0].x, old[0].y, old[0].z, old[0].w,
				old[2].x, old[2].y, old[2].z, old[1].w,
				old[1].x, old[1].y, old[1].z, old[2].w,
				old[3].x, old[3].y, old[3].z, old[3].w);
	return m;
}
