#ifndef HUD_H
#define HUD_H

#include <iostream>
#include "PostProcessing.h"

using namespace std;

class HUD {

public:
	HUD();
	~HUD();
	void init();
	void draw();
	string buttonText;
	PostProcessing* postProcessing;

};

#endif