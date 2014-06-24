#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "corona.h"
#include "globals.h"
#include <stdlib.h> //splitpath
#include <random>
#include <time.h>       /* time for rand() */


class Image {
public:
	Image::Image();
	Image(std::string filename);
	Image(int width, int height, std::string fileName);
	bool Exists();
	int getWidth();
	int getHeight();
	std::string getFileName();
	std::string getFileNameWithoutExtension();
	bool Image::saveToFile(std::string filename, ColorEnum color);
	void convertToColor(ColorEnum color);
	int addNoise(int amount, noiseTypeEnum noise); // Returns amount of bits flipped.
	~Image();
	Image::Image(const Image& image);
	byte* getImageData(ColorEnum color);
	void Image::setImageData(ColorEnum color, byte* imageData);
	Pixel* getImagePixelData();
	void Image::setImagePixelData(Pixel* imageData);
private:
	int imageWidth;
	int imageHeight;
	corona::Image* inputImage;
	std::string filename;

	byte* grayData; 
	byte* redData;
	byte* greenData;
	byte* blueData;
	Pixel* colorData;

	//bitmap file headers ( not used at this moment )
	typedef struct bitmapFileHeader
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
	} BITMAPINFOHEADER;
};

