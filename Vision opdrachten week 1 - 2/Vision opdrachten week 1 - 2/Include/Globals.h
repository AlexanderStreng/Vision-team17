#pragma once
#include <vector>

typedef unsigned char byte;

enum ColorEnum { GRAYSCALE, RED, GREEN, BLUE, INVERTED, FULLCOLOUR };
enum filterTypeEnum { MIN, MAX, MEAN, MEDIAN, GAUSSIAN, SHARPEN, SOFTEN, DYNAMIC };
enum noiseTypeEnum { SALTANDPEPPER, GAUSSIANNOISE };

struct Pixel
{
	Pixel(){}
	Pixel(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a){}
	byte r, g, b, a;
}; //alpha might come in handy later

struct Mean
{
	Mean(){}
	Mean(Pixel centerPixel, int label) : centerPixel(centerPixel), label(label){}
	Pixel centerPixel;
	Pixel lastCenterPixel;
	std::vector<Pixel*> assignedPixels;
	int label;
};