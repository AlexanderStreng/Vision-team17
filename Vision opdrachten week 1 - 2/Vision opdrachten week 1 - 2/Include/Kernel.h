#pragma once
#include "globals.h"
#include <math.h>       /* sqrt */


class Kernel
{
public:
	Kernel(int kernelSize, int kernelFactor, filterTypeEnum kernelType);
	void setFactor(int factor);
	void initializeKernel();
	int getKernelSize();
	double* getKernelData();
	filterTypeEnum getKernelFilterType();
	~Kernel(void);
private:
	int kernelSize; // e.g. 3 to create a 3x3 kernel
	int kernelFactor;
	filterTypeEnum kernelType;
	double* kernel;  //might use a vector.
};

