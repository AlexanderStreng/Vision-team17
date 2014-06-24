#pragma once
#include "globals.h"
#include "Image.h"
#include <math.h>       /* sqrt */
#include <algorithm>    // std::max
#include <climits> // for MAX_INT

class Kernel;


class Filter {
public:
	Filter(Image* image, int kernelSize, ColorEnum color);
	void ApplyFilter(filterTypeEnum kernelType, int passes);
	~Filter(void);
private:
	int kernelSize, totalKernelSize, offset;
	Image* imagePtr;
	int imageWidth, imageHeight;
	byte* imageData;
	byte* imageSourceData;
	inline byte DoMinFilter(int x, int y);
	inline byte DoMaxFilter(int x, int y);
	inline byte DoMedianFilter(int x, int y);
};

