#pragma once
#include <string>
#include "globals.h"

#include "Image.h"

class Histogram
{
public:
	Histogram(int bins, ColorEnum color, Image * image);
	~Histogram();
	bool saveHistogramAsCSV(std::string prefix);
	bool EqualizeImage();
private:
	int imageWidth;
	int imageHeight;
	Image* imagePtr;
	byte* imageData;
	int bins;
	int* histogramBins;
	ColorEnum imageColor;
	void calculateBins();
};

