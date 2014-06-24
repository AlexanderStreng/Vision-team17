#pragma once

#include "globals.h"

class Mean
{
	public:
		Mean(void){}
		~Mean(void){}
		Mean(Pixel centerPixel, int label) : centerPixel(centerPixel), label(label){}
		Pixel centerPixel;
		Pixel lastCenterPixel;
		std::vector<Pixel*> assignedPixels;
		int label;
};

