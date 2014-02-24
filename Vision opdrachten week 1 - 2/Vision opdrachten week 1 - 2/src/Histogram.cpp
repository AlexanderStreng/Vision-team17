#include "Histogram.h"


Histogram::Histogram(int bins, ColorEnum color, Image * image) :
	bins(bins),
	imageColor(color),
	histogramBins(0)
{
	imagePtr = image;
	imageWidth = imagePtr->getWidth();
	imageHeight = imagePtr->getHeight();
	imageData = imagePtr->getImageData(color); //pointer to our data.

	histogramBins = new int[bins](); //allocate some space.
	calculateBins();
}

Histogram::~Histogram(void)
{
	delete [] histogramBins;
}

void Histogram::calculateBins()
{	
	for (int i = 0; i < imageWidth * imageHeight; ++i) 
	{
		int val = 0;
		histogramBins[(int)(floor(((imageData[i] * bins) / 256) + 0.5))]++;//floor+0.5 for proper rounding
	}
}

bool Histogram::EqualizeImage()
{
	long imageSize = (imageWidth * imageHeight);
	//scaling factor, x = 255 / number of pixels
	float scale = (float)bins / (float)imageSize;

	int* lookupHistogram = new int[bins]();

	// Build Cumulative Histogram which will be our lookup table 
	lookupHistogram[0] = histogramBins[0] * scale; 
	int sum = 0;
	for (int i = 1; i < bins; i++) { 
		sum += histogramBins[i];
		lookupHistogram[i] = sum * scale;
	}

	//now reassign all pixelss
	for (int i = 0; i < imageSize; i++) 
	{ 
		int newValue = lookupHistogram[(int)imageData[i]];
		imageData[i] = (newValue > 255) ? (int)(255) : (int)(newValue); 
	} 

	delete [] lookupHistogram; //cleanup
	return true;
}

bool Histogram::saveHistogramAsCSV(std::string prefix)
{
	std::ofstream csvFile;
	std::stringstream ss;
	ss << bins << "_" << prefix << "_" << imagePtr->getFileNameWithoutExtension() << ".csv";
	csvFile.open(ss.str()); 
	csvFile << "binnr" << "," << "Density"  << std::endl; 

	for (int i = 0 ; i < bins; i++) 
	{ 
		csvFile << i << "," << (int)histogramBins[i]  << std::endl; 
	} 

	csvFile.close(); 
	return true;
}
