#include "Kernel.h"


Kernel::Kernel(int kernelSize, int kernelFactor, filterTypeEnum kernelType):
	kernelSize(kernelSize),
	kernelFactor(kernelFactor),
	kernelType(kernelType) {
	initializeKernel();
}


Kernel::~Kernel(void) {
}

void Kernel::initializeKernel() {
	int totalKernelSize = kernelSize * kernelSize;
}

double* Kernel::getKernelData() {
	return kernel;
}

filterTypeEnum Kernel::getKernelFilterType() {
	return kernelType;
}

int Kernel::getKernelSize() {
	return kernelSize;
}

void Kernel::setFactor(int factor) {
	kernelFactor = factor;
}