#pragma once

#include "globals.h"
#include "Image.h"
#include <algorithm>

class Treshold
{
public:
	Treshold(Image *image);
	~Treshold(void);
	int determineTreshHold();
	void doTreshHold(int treshHold);
private:
	int imageWidth, imageHeight;
	Image* imagePtr;
	byte* imageGrayData;
};

