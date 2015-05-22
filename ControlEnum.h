#ifndef CONTROL_ENUM_H
#define CONTROL_ENUM_H

#include <string>

enum ControlEnum
{
	None,
	Joystick,
	JoystickC,
	JoystickZ,
	Joystick1,
	Joystick2,
	Joystick3,
	Joystick4
};

ControlEnum StringToControlEnum(std::string str);

std::string ControlEnumToString(ControlEnum controlEnum);

#endif