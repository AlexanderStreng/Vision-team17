#include "Image.h"

Image::Image(){};

Image::Image(std::string filename) :
	_filename(filename)
{
	corona::Image* inputImage = corona::OpenImage(_filename.c_str(), corona::PF_R8G8B8A8); //Open the image using Corona lib

	width  = inputImage->getWidth();
	height = inputImage->getHeight();

	void* pixels = inputImage->getPixels();

	byte* p = (byte*)pixels;

	for (int i = 0; i < width * height; ++i) 
	{
		byte red   = *p++;
		byte green = *p++;
		byte blue  = *p++;
		byte alpha = *p++;
	}
	//still need to copy the data to an internal byte vector
}

Image::~Image()
{
}

bool Image::saveToFile(std::string filename)
{
	
}

bool Image::Excists()
{
	return inputImage;
}

int Image::getWidth(){
	return width;
}

int Image::getHeight(){
	return height;
}


