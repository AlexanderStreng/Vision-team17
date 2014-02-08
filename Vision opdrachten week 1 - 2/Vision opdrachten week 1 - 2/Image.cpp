#include "Image.h"

Image::Image(){};

Image::Image(int width, int height, void* pixels) :
	_width(width),
	_height(height)
{
	setPixels(pixels);
}

void Image::setPixels(void* pixels)
{
	byte* p = (byte*)pixels;

	for (int i = 0; i < _width * _height; ++i) 
	{
		byte red   = *p++;
		byte green = *p++;
		byte blue  = *p++;
		byte alpha = *p++;
	}
}

Image::~Image()
{
}
