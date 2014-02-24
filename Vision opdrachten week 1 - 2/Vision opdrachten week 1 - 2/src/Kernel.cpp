#include "Kernel.h"


Kernel::Kernel(int kernelSize, int kernelFactor, kernelTypeEnum kernelType):
	kernelSize(kernelSize),
	kernelFactor(kernelFactor),
	kernelType(kernelType) {
	initializeKernel();
}


Kernel::~Kernel(void) {
}

void Kernel::initializeKernel() {
	int totalKernelSize = kernelSize * kernelSize;
	kernel = new double[totalKernelSize](); //allocate some space.

	switch(kernelType) {
	case MIN:
		break;
	case MAX:
		break;
	case MEAN:
		for(int i = 0; i < totalKernelSize; ++i) {
			kernel[i] = 1 / totalKernelSize;
		}
		break;
	case MEDIAN:
		break;
	case DYNAMIC:
		break;
	default:
		//throw notImplemented()
		break;
	}
}

int Kernel::getKernelSize() {
	return kernelSize;
}

void Kernel::setFactor(int factor) {
	kernelFactor = factor;
}