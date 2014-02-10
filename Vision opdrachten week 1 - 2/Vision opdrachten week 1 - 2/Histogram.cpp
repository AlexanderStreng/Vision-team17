#include "histogram.h"

Histogram::Histogram(int bins):
	bins(bins){}

bool Histogram::calculateGrayBins(Image image) //presuming this is NOT a grayscale image!
{	
	dataGray = new int[bins]();

	unsigned char* p = (unsigned char*)image.getPixels();

	for (int i = 0; i < image.getWidth() * image.getHeight(); ++i) 
	{
		int r = (int)*p++; 
		int g = (int)*p++; 
		int b = (int)*p++;
		int intensity = (int)((r+g+b) / 3);
		int val = floor((((intensity * bins)) / 256) + 0.5); //floor+0.5 for proper rounding
		dataGray[val]++;
	}
	return saveAsCSV(image.getFileNameWithoutExtension(), Image::GRAYSCALE);
}

bool Histogram::saveAsCSV(std::string filename, Image::OutputColorEnum color)
{
	std::ofstream csvFile;
	std::stringstream ss;
	ss << bins << "_" << color << "_" << filename << ".txt";
	csvFile.open(ss.str()); 
	csvFile << "binnr" << "," << "Density"  << std::endl; 

	for (int i = 0 ; i < bins; i++) 
	{ 
		csvFile << i << "," << (int)dataGray[i]  << std::endl; 
	} 

	csvFile.close(); 
	return true;
}

Histogram::~Histogram()
{

}
