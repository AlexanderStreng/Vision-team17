#include "Filter.h"


Filter::Filter(Image* image, Kernel* kernel) :
	imagePtr(image),
	kernelPtr(kernel) {
}

void Filter::ApplyFilter(ColorEnum color) {
	int imageWidth = imagePtr->getWidth();
	int imageHeight = imagePtr->getHeight();
	double* kernel = kernelPtr->getKernelData();
	int kernelSize = kernelPtr->getKernelSize();
	int totalKernelSize = kernelSize * kernelSize;
	int offset = kernelSize / 2;

	/* Left, right, top and bottom limits */
	const int left = offset, top = offset, right = std::max(imageWidth - offset, left), bottom = std::max(imageHeight - offset, top);

	if(color == FULLCOLOUR) { // needs to apply filters to all 3 channels. Lets go multithreading!

	} else {

		//MEAN filtering.

		byte* imageData = imagePtr->getImageData(color);
		byte* imageDataSource = new byte[imageWidth*imageHeight];
		memcpy(imageDataSource, imageData, (imageWidth * imageHeight) * sizeof(byte)); // duur. auch.

		int sum;
		/* Iterate over pixels of image */
		for(int y = top; y < bottom; y++) {
			for(int x = left; x < right; x++) {

				sum = 0;
				for(int i = -offset; i <= offset; i++){ //mask
					for(int j = -offset; j <= offset; j++) { //mask
						sum += imageDataSource[((y + i) * imageWidth) + (x + j)];
					}
				}
				imageData[(y * imageWidth) + x] = (byte)(sum / totalKernelSize);
			}
		}
		delete [] imageDataSource;//cleanup
	}

}

Filter::~Filter(void) {
}
