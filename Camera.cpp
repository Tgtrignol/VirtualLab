#include "Camera.h"

#include <GL\glew.h>

#include <stdlib.h>
#include <algorithm>

using namespace std;

float fpXAngle = 90.0;
float fpYAngle = 0.0;
float fpCameraXCoordinate = 0.0;
float fpCameraYCoordinate = DISTANCE_OF_HEAD_FROM_FLOOR;
float fpCameraZCoordinate = -0.3;

/*
* 	Function:  InitCamera
* 	--------------------
* 	Rotates the world so the camera effect is enabled, this should be called prior to InitCameraTranslation
*
*  returns: void
*/
void InitCameraRotation()
{
	glRotatef(-fpYAngle, 1.0, 0.0, 0.0);
	glRotatef(-fpXAngle, 0.0, 1.0, 0.0);
}

/*
* 	Function:  InitCamera
* 	--------------------
* 	Translates the world so the camera effect is enabled this should be called after InitCameraRotation
*
*  returns: void
*/
void InitCameraTranslation()
{
	glTranslatef(-fpCameraXCoordinate, -fpCameraYCoordinate, -fpCameraZCoordinate);
}

/*
* 	Function:  MoveForward
* 	--------------------
* 	Moves the camera forward in it's current direction by one step
*
*  returns: void
*/
void MoveForward()
{
	float fpXRotationInRadians = (fpXAngle / 180 * PI);

	fpCameraXCoordinate += float(-sin(fpXRotationInRadians)) * CAMERA_SPEED;
	fpCameraZCoordinate += float(-cos(fpXRotationInRadians)) * CAMERA_SPEED;
}

/*
* 	Function:  MoveBackward
* 	--------------------
* 	Moves the camera backward in it's current direction by one step
*
*  returns: void
*/
void MoveBackward()
{
	float fpXRotationInRadians = (fpXAngle / 180 * PI);

	fpCameraXCoordinate -= float(-sin(fpXRotationInRadians)) * CAMERA_SPEED;
	fpCameraZCoordinate -= float(-cos(fpXRotationInRadians)) * CAMERA_SPEED;
}

/*
* 	Function:  MoveRight
* 	--------------------
* 	Moves the camera to the right in it's current direction by one step
*
*  returns: void
*/
void MoveRight()
{
	float fpXRotationInRadians = (fpXAngle / 180 * PI);

	fpCameraXCoordinate += float(cos(fpXRotationInRadians)) * CAMERA_SPEED;
	fpCameraZCoordinate += float(-sin(fpXRotationInRadians)) * CAMERA_SPEED;
}

/*
* 	Function:  MoveLeft
* 	--------------------
* 	Moves the camera to the left in it's current direction by one step
*
*  returns: void
*/
void MoveLeft()
{
	float fpXRotationInRadians = (fpXAngle / 180 * PI);

	fpCameraXCoordinate -= float(cos(fpXRotationInRadians)) * CAMERA_SPEED;
	fpCameraZCoordinate -= float(-sin(fpXRotationInRadians)) * CAMERA_SPEED;
}

/*
* 	Function:  MoveUpward
* 	--------------------
* 	Moves the camera upward in it's current direction by one step
*
*  returns: void
*/
void MoveUpward()
{
	fpCameraYCoordinate += CAMERA_SPEED;
}

/*
* 	Function:  MoveDownward
* 	--------------------
* 	Moves the camera downward in it's current direction by one step
*
*  returns: void
*/
void MoveDownward()
{
	fpCameraYCoordinate -= CAMERA_SPEED;
}

/*
* 	Function:  RotateRight
* 	--------------------
* 	Rotates the camera to the right in it's current direction by one degree
*
*  returns: void
*/
void RotateRight()
{
	fpXAngle -= CAMERA_ROTATION_SPEED;
}

/*
* 	Function:  RotateLeft
* 	--------------------
* 	Rotates the camera to the left in it's current direction by one degree
*
*  returns: void
*/
void RotateLeft()
{
	fpXAngle += CAMERA_ROTATION_SPEED;
}

/*
* 	Function:  RotateUp
* 	--------------------
* 	Rotates the camera up in it's current direction by one degree
*
*  returns: void
*/
void RotateUp()
{
	fpYAngle += CAMERA_ROTATION_SPEED;
}

/*
* 	Function:  RotateDown
* 	--------------------
* 	Rotates the camera down in it's current direction by one degree
*
*  returns: void
*/
void RotateDown()
{
	fpYAngle -= CAMERA_ROTATION_SPEED;
}