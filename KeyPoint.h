#ifndef KEY_POINT_H
#define KEY_POINT_H

#include <string>

struct KeyPoint{
public:
	std::string m_name;
	bool m_isSuccessTriggered;

	KeyPoint(std::string name, bool isSuccessTriggered) :
		m_name(name), m_isSuccessTriggered(isSuccessTriggered) { }
};

#endif