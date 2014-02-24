#pragma once
#include "globals.h"
#include "Image.h"

class Kernel;


class Filter {
public:
	Filter(Image * image, const Kernel * kernel);
	bool ApplyFilter(ColorEnum color);
	~Filter(void);
private:
	Image * imagePtr;
	const Kernel * kernelPtr;
};

