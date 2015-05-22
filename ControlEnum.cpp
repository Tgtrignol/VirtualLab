#include "ControlEnum.h"

ControlEnum StringToControlEnum(std::string str)
{
	if (str == "None")
		return None;

	if (str == "Joystick-C")
		return JoystickC;

	if (str == "Joystick-Z")
		return JoystickZ;

	if (str == "Joystick-1")
		return Joystick1;

	if (str == "Joystick-2")
		return Joystick2;

	if (str == "Joystick-3")
		return Joystick3;

	if (str == "Joystick-4")
		return Joystick4;

	return Joystick;
}

std::string ControlEnumToString(ControlEnum controlEnum)
{
	switch (controlEnum)
	{
	case None:
		return "None";
	case JoystickC:
		return "Joystick-C";
	case JoystickZ:
		return "Joystick-Z";
	case Joystick1:
		return "Joystick-1";
	case Joystick2:
		return "Joystick-2";
	case Joystick3:
		return "Joystick-3";
	case Joystick4:
		return "Joystick-4";
	default:
		return "Joystick";
	}
}