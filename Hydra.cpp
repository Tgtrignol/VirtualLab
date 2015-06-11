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
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp>
#include "ProcedureManager.h"
#include <iostream>

void Hydra::init()
{
	shaderID = initShader("lightingShader");

	hydraRightPosition.init("RightNunchuck");
	hydraLeftPosition.init("LeftNunchuck");
	hydraRightJoystick.init("RightJoystick");
	hydraLeftJoystick.init("LeftJoystick");

	hydraLeftTrigger.init("LeftTrigger");
	hydraLeftBumper.init("LeftBumper");
	hydraRightTrigger.init("RightTrigger");
	hydraRightBumper.init("RightBumper");

	hydraLeftOne.init("LeftButtonOne");
	hydraRightOne.init("RightButtonOne");
	hydraLeftTwo.init("LeftButtonTwo");
	hydraRightTwo.init("RightButtonTwo");
	hydraLeftThree.init("LeftButtonThree");
	hydraRightThree.init("RightButtonThree");
	hydraLeftFour.init("LeftButtonFour");
	hydraRightFour.init("RightButtonFour");

	

	initHydraModels();
}

void Hydra::draw(float InitialModelView[16])
{
	if (!hydraEnabled)
	{
		return;
	}

	glColor3f(1, 1, 0);

	//Hydra Laser
	float cameraYAngle = TO_RADIANS(-fpXAngle);
	float cameraXAngle = TO_RADIANS(-fpYAngle);

	//Left
	{
		glBegin(GL_LINES);

		glm::mat4 tra;
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
		tra = glm::translate(tra, glm::vec3(hydraLeftPositionVector.x * 2.0f,
			hydraLeftPositionVector.y * 2.0f - 1.0f,
			hydraLeftPositionVector.z * 2.0f - 2.0f));

		glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(tran.x, tran.y, tran.z);

		tra = glm::mat4();
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
		tra = glm::translate(tra, glm::vec3(hydraLeftPositionVector.x * 2.0f + hydraLeftOrientation.x,
			hydraLeftPositionVector.y * 2.0f - 1.0f + hydraLeftOrientation.y,
			hydraLeftPositionVector.z * 2.0f - 2.0f + hydraLeftOrientation.z));
		tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(tran.x, tran.y, tran.z);

		glEnd();
	}

	//Right
	{
		glBegin(GL_LINES);

		glm::mat4 tra;
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
		tra = glm::translate(tra, glm::vec3(hydraRightPositionVector.x * 2.0f,
			hydraRightPositionVector.y * 2.0f - 1.0f,
			hydraRightPositionVector.z * 2.0f - 2.0f));

		glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(tran.x, tran.y, tran.z);

		tra = glm::mat4();
		tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
		tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
		tra = glm::translate(tra, glm::vec3(hydraRightPositionVector.x * 2.0f + hydraRightOrientation.x,
			hydraRightPositionVector.y * 2.0f - 1.0f + hydraRightOrientation.y,
			hydraRightPositionVector.z * 2.0f - 2.0f + hydraRightOrientation.z));
		tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(tran.x, tran.y, tran.z);

		glEnd();
	}
	//End Hydra Laser

	glColor3f(0, 0, 0);

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
			GameManager::getInstance()->scene->world->addRigidBody(rightModel->rigidBody);
			GameManager::getInstance()->scene->world->addRigidBody(leftModel->rigidBody);
			initRigidbodies = true;
		}

		glm::mat4 hydraMatrix = hydraRightPosition.getData();
		hydraRightPositionVector = hydraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // The origin of the razer hydra.
		hydraRightOrientation = glm::normalize((hydraMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f)) - hydraRightPositionVector); // Where the razer hydra point to.

		{
			float cameraYAngle = TO_RADIANS(-fpXAngle);
			float cameraXAngle = TO_RADIANS(-fpYAngle);

			glm::mat4 tra;
			tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
			tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
			tra = glm::translate(tra, glm::vec3(
				hydraRightPositionVector.x * 2.0f,
				hydraRightPositionVector.y * 2.0f - 1.0f,
				hydraRightPositionVector.z * 2.0f - 2.0f));

			glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			btVector3 btFrom(tran.x, tran.y, tran.z);

			tra = glm::mat4();
			tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
			tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
			tra = glm::translate(tra, glm::vec3(
				hydraRightPositionVector.x * 2.0f + hydraRightOrientation.x,
				hydraRightPositionVector.y * 2.0f - 1.0f + hydraRightOrientation.y,
				hydraRightPositionVector.z * 2.0f - 2.0f + hydraRightOrientation.z)); //TODO: Add stable point of standing parameters

			tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			btVector3 btTo(tran.x, tran.y, tran.z);


			btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);
			hydraRightVector = btFrom;

			GameManager::getInstance()->scene->world->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

			if (res.hasHit()){
				if (res.m_collisionObject->getUserPointer() != this)
				{
					GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject = (ProcedureObject *)res.m_collisionObject->getUserPointer();
				}
			}
			else
			{
				GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject = nullptr;
			}
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
		tra = glm::translate(tra, glm::vec3(hydraRightPositionVector.x * 2.0f, hydraRightPositionVector.y * 2.0f - 1.0f, hydraRightPositionVector.z * 2.0f - 2.0f));
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

		{
			float cameraYAngle = TO_RADIANS(-fpXAngle);
			float cameraXAngle = TO_RADIANS(-fpYAngle);

			glm::mat4 tra;
			tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
			tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
			tra = glm::translate(tra, glm::vec3(
				hydraLeftPositionVector.x * 2.0f,
				hydraLeftPositionVector.y * 2.0f - 1.0f,
				hydraLeftPositionVector.z * 2.0f - 2.0f));

			glm::vec4 tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			btVector3 btFrom(tran.x, tran.y, tran.z);

			tra = glm::mat4();
			tra = glm::translate(tra, glm::vec3(fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate));
			tra = glm::rotate(tra, -cameraYAngle, glm::vec3(0.0f, 1.0f, 0.0f));
			tra = glm::rotate(tra, -cameraXAngle, glm::vec3(1, 0, 0));
			tra = glm::translate(tra, glm::vec3(
				hydraLeftPositionVector.x * 2.0f + hydraLeftOrientation.x,
				hydraLeftPositionVector.y * 2.0f - 1.0f + hydraLeftOrientation.y,
				hydraLeftPositionVector.z * 2.0f - 2.0f + hydraLeftOrientation.z)); //TODO: Add stable point of standing parameters

			tran = tra * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			btVector3 btTo(tran.x, tran.y, tran.z);


			btCollisionWorld::ClosestRayResultCallback res(btFrom, btTo);
			hydraLeftVector = btFrom;

			GameManager::getInstance()->scene->world->rayTest(btFrom, btTo, res); // m_btWorld is btDiscreteDynamicsWorld

			if (res.hasHit()){
				if (res.m_collisionObject->getUserPointer() != this)
				{
					GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject = (ProcedureObject *)res.m_collisionObject->getUserPointer();
				}
			}
			else
			{
				GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject = nullptr;
			}
		}

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
		//if (hydraLeftBumper.getData() == DigitalState::ON || hydraLeftBumper.getData() == DigitalState::TOGGLE_ON)
			//MoveUpward();
	}
	if (hydraLeftTrigger.isInitialized())
	{
		//if (hydraLeftTrigger.getData() - 0.1f > 0.0f)
			//MoveDownward();
	}
}

void Hydra::initHydraModels()
{
	rightModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\Sword01\\rusword.obj", 0, btVector3(0, -100, 0), btVector3(0.0018, 0.0018, 0.0018)); //TODO: Adjust scale
	rightModel->rigidBody->setUserPointer(this);
	leftModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\Sword02\\rusword.obj", 0, btVector3(0, -100, 0), btVector3(0.0018, 0.0018, 0.0018));
	leftModel->rigidBody->setUserPointer(this);
}

btVector3* Hydra::getRightHydraCor() {

	return &hydraRightVector;
}

btVector3* Hydra::getLeftHydraCor() {

	return &hydraLeftVector;
}

glm::mat4 Hydra::getWorldMatrixFromHydra(glm::mat4 old)
{
	glm::mat4 m(old[0].x, old[0].y, old[0].z, old[0].w,
				old[2].x, old[2].y, old[2].z, old[1].w,
				old[1].x, old[1].y, old[1].z, old[2].w,
				old[3].x, old[3].y, old[3].z, old[3].w);
	return m;
}

std::string Hydra::checkButtons()
{
	if (hydraLeftOne.getData() == DigitalState::TOGGLE_ON || hydraLeftOne.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-1"; 
		}
	}
	else if (hydraRightOne.getData() == DigitalState::TOGGLE_ON || hydraRightOne.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-1";
		}
	}
	else if (hydraLeftTwo.getData() == DigitalState::TOGGLE_ON || hydraLeftTwo.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-2";
		}
	}
	else if (hydraRightTwo.getData() == DigitalState::TOGGLE_ON || hydraRightTwo.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-2";
		}
	}
	else if (hydraLeftThree.getData() == DigitalState::TOGGLE_ON || hydraLeftThree.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-3";
		}
	}
	else if (hydraRightThree.getData() == DigitalState::TOGGLE_ON || hydraRightThree.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-3";
		}
	}
	else if (hydraLeftFour.getData() == DigitalState::TOGGLE_ON || hydraLeftFour.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-4";
		}
	}
	else if (hydraRightFour.getData() == DigitalState::TOGGLE_ON || hydraRightFour.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-4";
		}
	}
	else if (hydraLeftBumper.getData() == DigitalState::TOGGLE_ON || hydraLeftBumper.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-C";
		}
	}
	else if (hydraRightBumper.getData() == DigitalState::TOGGLE_ON || hydraRightBumper.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-C|Right";
		}
	}
	else if (hydraLeftTrigger.getData() == DigitalState::TOGGLE_ON || hydraLeftTrigger.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Left";
			return "Joystick-Z";
		}
	}
	else if (hydraRightTrigger.getData() == DigitalState::TOGGLE_ON || hydraRightTrigger.getData() == DigitalState::ON)
	{
		if (!hydraPressed)
		{
			hydraPressed = true;
			GameManager::getInstance()->scene->procedureManager->RightLeft = "Right";
			return "Joystick-Z";
		}
	}
	else if (hydraLeftOne.getData() == DigitalState::OFF || hydraRightOne.getData() == DigitalState::OFF || hydraLeftTwo.getData() == DigitalState::OFF || hydraRightTwo.getData() == DigitalState::OFF || hydraLeftThree.getData() == DigitalState::OFF || hydraRightThree.getData() == DigitalState::OFF || hydraLeftFour.getData() == DigitalState::OFF || hydraRightFour.getData() == DigitalState::OFF || hydraLeftBumper.getData() == DigitalState::OFF || hydraRightBumper.getData() == DigitalState::OFF || hydraLeftTrigger.getData() == DigitalState::OFF || hydraRightTrigger.getData() == DigitalState::OFF)
	{
		if (hydraPressed)
		{
			GameManager::getInstance()->scene->procedureManager->RightLeft = "None";
			hydraPressed = false;
		}
	}
	return "None";
}
