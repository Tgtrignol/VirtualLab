#ifndef KEY_POINT_H
#define KEY_POINT_H

#include <string>
#include <vector>

struct KeyPoint{
public:
	std::string m_name;
	bool  m_isSuccessTriggered;
	std::vector<std::string *> m_params;
	std::string m_primitive;

	KeyPoint(std::string name, bool isSuccessTriggered, std::string primitive, std::vector<std::string *> params) :
		m_name(name), m_isSuccessTriggered(isSuccessTriggered), m_primitive(primitive), m_params(params) { }
};

#endif