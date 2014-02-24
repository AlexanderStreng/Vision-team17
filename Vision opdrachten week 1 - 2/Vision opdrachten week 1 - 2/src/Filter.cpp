#include "Filter.h"


Filter::Filter(Image * image, const Kernel * kernel) :
imagePtr(image),
kernelPtr(kernel) {}

bool Filter::ApplyFilter(ColorEnum color) {
	//Pixel * imageData = imagePtr->getImageData(); //pointer to our data. (which is WxHx3 byte in mem)
	return true;
}

Filter::~Filter(void) {
}
