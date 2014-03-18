#include "Filter.h"


Filter::Filter(Image* image, Kernel* kernel) :
	imagePtr(image),
	kernelPtr(kernel) {
}

void Filter::ApplyFilter(filterTypeEnum kernelType, ColorEnum color, int passes) {
	int imageWidth = imagePtr->getWidth();
	int imageHeight = imagePtr->getHeight();
	double* kernelData = kernelPtr->getKernelData();
	int kernelSize = kernelPtr->getKernelSize();
	int totalKernelSize = kernelSize * kernelSize;
	int offset = kernelSize / 2;

	/* Left, right, top and bottom limits */
	const int left = offset, top = offset, right = std::max(imageWidth - offset, left), bottom = std::max(imageHeight - offset, top);


	for(int p = 0; p < passes; p++) { //allowing multiple passes

		byte* imageData = imagePtr->getImageData(color);
		byte* imageDataSource = new byte[imageWidth*imageHeight];
		memcpy(imageDataSource, imageData, (imageWidth * imageHeight) * sizeof(byte));

		//donnt want to switch inside the pixelloop.
		switch(kernelType) {
		case MIN:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMinFilter(imageData, offset, x, y, imageWidth, imageHeight, totalKernelSize);
				}
			}
			break;
		case MAX:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMaxFilter(imageData, offset, x, y, imageWidth, imageHeight, totalKernelSize);
				}
			}
			break;
		case MEAN:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMeanFilter(imageData, offset, x, y, imageWidth, imageHeight, totalKernelSize);
				}
			}
			break;
		case MEDIAN:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoMedianFilter(imageData, offset, x, y, imageWidth, imageHeight, totalKernelSize);
				}
			}
			break;
		case DYNAMIC:
			for(int y = top; y < bottom; y++) {
				for(int x = left; x < right; x++) {
					imageData[(y * imageWidth) + x] = DoDynamicFilter(imageData, offset, x, y, imageWidth, imageHeight, totalKernelSize);
				}
			}
			break;
		default:
			//throw notImplemented()
			break;
		}

		delete [] imageDataSource;//cleanup
	}
}

inline byte Filter::DoMeanFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize) {
	int sum = 0;
	for(int i = -maskOffset; i <= maskOffset; i++){ //mask
		for(int j = -maskOffset; j <= maskOffset; j++) { //mask
			sum += imageDataSource[((y + i) * imageWidth) + (x + j)];
		}
	}
	return (byte)(sum / totalKernelSize);
}

inline byte Filter::DoMinFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize) {
	int lowest = 0;
	for(int i = -maskOffset; i <= maskOffset; i++){ //mask
		for(int j = -maskOffset; j <= maskOffset; j++) { //mask
			int value = imageDataSource[((y + i) * imageWidth) + (x + j)];
			lowest = value < lowest ? value : lowest;
		}
	}
	return (byte)lowest;
}

inline byte Filter::DoMaxFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize) {
	int highest = 0;
	for(int i = -maskOffset; i <= maskOffset; i++){ //mask
		for(int j = -maskOffset; j <= maskOffset; j++) { //mask
			int value = imageDataSource[((y + i) * imageWidth) + (x + j)];
			highest = value > highest ? value : highest;
		}
	}
	return (byte)highest;
}

inline byte Filter::DoMedianFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize) {
	std::vector<int> values (totalKernelSize, 0); //might want to switch to an array.
	int index = 0;
	for(int i = -maskOffset; i <= maskOffset; i++){ //mask
		for(int j = -maskOffset; j <= maskOffset; j++) { //mask
			values[index++] = imageDataSource[((y + i) * imageWidth) + (x + j)];
		}
	}
	size_t size = values.size();
	std::sort (values.begin(), values.end());
	double median = size % 2 ? values[size / 2] : (values[size / 2 - 1] + values[size / 2]) / 2;
	return (byte)median;
}

inline byte Filter::DoDynamicFilter(byte* imageDataSource, int maskOffset, int x, int y, int imageWidth, int imageHeight, int totalKernelSize) {
	int sum = 0;
	for(int i = -maskOffset; i <= maskOffset; i++){ //mask
		for(int j = -maskOffset; j <= maskOffset; j++) { //mask
			sum += imageDataSource[((y + i) * imageWidth) + (x + j)];
		}
	}
	//imageData[(y * imageWidth) + x] = (byte)(sum / totalKernelSize);
	return 0;
}


Filter::~Filter(void) {
}
