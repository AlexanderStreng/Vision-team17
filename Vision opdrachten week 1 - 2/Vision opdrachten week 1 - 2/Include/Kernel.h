#pragma once
#include "globals.h"
#include <math.h>       /* sqrt */


class Kernel
{
public:
	Kernel(int kernelSize, int kernelFactor, kernelTypeEnum kernelType);
	void setFactor(int factor);
	void initializeKernel();
	int getKernelSize();
	~Kernel(void);
private:
	int kernelSize; // e.g. 3 to create a 3x3 kernel
	int kernelFactor;
	kernelTypeEnum kernelType;
	double* kernel;  //might use a vector.
};

