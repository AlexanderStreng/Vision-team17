#include "Filter.h"


Filter::Filter(Image* image, int kernelSize, ColorEnum color) :
	imagePtr(image),
	kernelSize(kernelSize) 
{
	imageWidth = imagePtr->getWidth();
	imageHeight = imagePtr->getHeight();

	imageData = imagePtr->getImageData(color);		

	imageSourceData = new byte[imageWidth * imageHeight];
	memcpy(imageSourceData, imageData, (imageWidth * imageHeight) * sizeof(byte)); // copy them image memories

	totalKernelSize = kernelSize * kernelSize;
	offset = kernelSize / 2;
}

void Filter::ApplyFilter(filterTypeEnum kernelType, int passes) {

	/* Left, right, top and bottom limits */
	const int left = offset, top = offset, right = std::max(imageWidth - offset, left), bottom = std::max(imageHeight - offset, top);

	for(int p = 0; p < passes; p++) { //allowing multiple passes

		//donnt want to switch inside the pixelloop.
		switch(kernelType) {
		case MIN:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMinFilter(x, y);
				}
			}
			break;
		case MAX:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMaxFilter(x, y);
				}
			}
			break;
		case MEDIAN:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMedianFilter(x, y);
				}
			}
			break;
		default:
			//throw notImplemented()
			break;
		}

	}
}


inline byte Filter::DoMinFilter(int x, int y) {
	int lowest = INT_MAX;
	for(int i = -offset; i <= offset; i++){ //mask
		for(int j = -offset; j <= offset; j++) { //mask
			int value = imageSourceData[((y + i) * imageWidth) + (x + j)];
			lowest = value < lowest ? value : lowest;
		}
	}
	return (byte)lowest;
}

inline byte Filter::DoMaxFilter(int x, int y) {
	int highest = INT_MIN;
	for(int i = -offset; i <= offset; i++){ //mask
		for(int j = -offset; j <= offset; j++) { //mask
			int value = imageSourceData[((y + i) * imageWidth) + (x + j)];
			highest = value > highest ? value : highest;
		}
	}
	return (byte)highest;
}

inline byte Filter::DoMedianFilter(int x, int y) {
	std::vector<int> values (totalKernelSize, 0); //might want to switch to an array.
	int index = 0;
	for(int i = -offset; i <= offset; i++){ //mask
		for(int j = -offset; j <= offset; j++) { //mask
			values[index++] = imageSourceData[((y + i) * imageWidth) + (x + j)];
		}
	}
	size_t size = values.size();
	std::sort (values.begin(), values.end());
	double median = size % 2 ? values[size / 2] : (values[size / 2 - 1] + values[size / 2]) / 2;
	return (byte)median;
}

Filter::~Filter(void) {
	delete [] imageSourceData; //cleanup
}
