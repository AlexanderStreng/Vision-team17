#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "corona.h"

typedef unsigned char byte;

class Image {
public:
	struct Pixel
	{
		Pixel(){}
		Pixel(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a){}
		byte r, g, b, a;
	}; //alpha might come in handy later
	enum OutputColorEnum { GRAYSCALE, RED, GREEN, BLUE, INVERTED };
	Image::Image();
	Image(std::string filename);
	bool Exists();
	int getWidth();
	int getHeight();
	int bins;
	int* histogramBins;
	std::string getFileName();
	std::string getFileNameWithoutExtension();
	bool Image::saveToFile(std::string filename);
	void convertToColor(OutputColorEnum color);
	bool saveHistogramAsCSV(int bins, std::string color);
	bool EqualizeImage(int bins);
	bool calculateBins(int bins, OutputColorEnum color);
	~Image();
	Image::Image(const Image& image);
private:
	int imageWidth;
	int imageHeight;
	corona::Image* inputImage;
	std::string filename;
	Pixel* imageData; //our pixels stored in memory

	//bitmap file headers
	/*typedef struct bitmapFileHeader
	{
	unsigned short int bfType;  // I think 0x4d42 goes here (otherwise known as the string 'BM')			  
	unsigned int bfSize;  // put sizeof(BITMAPFILEHEADER) here					 
	unsigned short int bfReserved1, bfReserved2; // These are 0
	unsigned int bfOffBits; // Return the size of the headers. This tells whatever reader where the image data starts			 
	} BITMAPFILEHEADER;

	typedef struct bitmapInfoHeader
	{
	unsigned int biSize; // sizeof(BITMAPINFOHEADER)
	int biWidth, biHeight; // Actual dimensions of the image			 
	unsigned short int biPlanes; // 1
	unsigned short int biBitCount; // 24  
	unsigned int biCompression; // = BI_RGB
	unsigned int biSizeImage; // put the actual byte size of the image here	  
	int biXPelsPerMeter, biYPelsPerMeter; // Put anything here	
	unsigned int biClrUsed; // For 24-bit, ignore these	 
	unsigned int biClrImportant; // Same
	} BITMAPINFOHEADER;*/

};

