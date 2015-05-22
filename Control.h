#ifndef CONTROL_H
#define CONTROL_H

#include <string>
#include "ControlEnum.h"

struct Control
{
	ControlEnum m_control;
	std::string m_primitive;

	Control(ControlEnum control, std::string primitive) : m_control(control), m_primitive(primitive) { }
};

#endif