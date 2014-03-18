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
	void ApplyFilter(filterTypeEnum kernelType, ColorEnum color, int passes);
	~Filter(void);
private:
	Image* imagePtr;
	Kernel* kernelPtr;
	inline byte DoMeanFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize);
	inline byte DoMinFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize);
	inline byte DoMaxFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize);
	inline byte DoMedianFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize);
	inline byte DoDynamicFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize);
};

