#ifndef STATIC_LAB_ENVIRONMENT_H
#define STATIC_LAB_ENVIRONMENT_H
#include "ObjModel.h"

#include <CaveLib\CaveLib\model.h>


class StaticLabEnvironment{
public:
	cModel *Labmodel;
	void init();
	void draw();
};

#endif