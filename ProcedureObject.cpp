#include <GL/glew.h>
#include "Control.h"
#include "ProcedureObject.h"
#include "Shader.h"
#include <btBulletCollisionCommon.h>
#include "ObjModel.h"
#include "Camera.h"

void ProcedureObject::init()
{
	shaderID = initShader("procedureObject");
	btVector3 size(0.2, 0.45, 1.0); //TODO: Should read this from file aswell
	btScalar mass = 0.0;
	pObjModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\"+fileName, size, mass, *origin);
	
	btTransform trans;
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

	glScalef(scale->x(), scale->y(), scale->z());

	GLint uniform = 0;

	uniform = glGetUniformLocation(shaderID, "useTexture");
	glUniform1i(uniform, !useColorInsteadOfTexture); 

	if (useColorInsteadOfTexture)
	{
		uniform = glGetUniformLocation(shaderID, "color");
		glUniform4f(uniform, color->x(), color->y(), color->z(), color->w());
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

}