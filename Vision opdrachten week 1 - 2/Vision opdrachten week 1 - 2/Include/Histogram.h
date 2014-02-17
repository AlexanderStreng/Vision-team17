#pragma once
#include <string>
#include "globals.h"

#include "Image.h"

class Histogram
{
public:
	Histogram(int bins, OutputColorEnum color, Image * image);
	~Histogram();
	bool saveHistogramAsCSV(std::string prefix);
	bool EqualizeImage();
private:
	int imageWidth;
	int imageHeight;
	Image* imagePtr;
	Pixel* imageData;
	int bins;
	int* histogramBins;
	OutputColorEnum imageColor;
	void calculateBins();
};

