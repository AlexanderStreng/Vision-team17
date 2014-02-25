#pragma once
#include "globals.h"
#include "Image.h"
#include "Kernel.h"
#include <math.h>       /* sqrt */
#include <algorithm>    // std::max

class Kernel;


class Filter {
public:
	Filter(Image* image, Kernel* kernel);
	void ApplyFilter(ColorEnum color);
	~Filter(void);
private:
	Image* imagePtr;
	Kernel* kernelPtr;
};

