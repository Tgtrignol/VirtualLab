#include "StaticLabEnvironment.h"

void StaticLabEnvironment::draw()
{
	glPushMatrix();
	Labmodel->draw(NULL);
	glPopMatrix();
}

void StaticLabEnvironment::init()
{
	Labmodel = CaveLib::loadModel("../Development/VirtualLab/Data/Lab/Lab_v0.2.obj");
}