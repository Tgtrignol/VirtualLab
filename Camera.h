#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.14159265358979323846f
#define CAMERA_SPEED 0.125f
#define CAMERA_ROTATION_SPEED 2.0f
#define DISTANCE_OF_HEAD_FROM_FLOOR 1.35f

extern void InitCameraRotation();
extern void InitCameraTranslation();
extern void MoveRight();
extern void MoveLeft();
extern void MoveForward();
extern void MoveBackward();
extern void MoveUpward();
extern void MoveDownward();
extern void RotateRight();
extern void RotateLeft();
extern void RotateUp();
extern void RotateDown();

extern float fpXAngle;
extern float fpYAngle;
extern float fpCameraXCoordinate;
extern float fpCameraYCoordinate;
extern float fpCameraZCoordinate;

#endif