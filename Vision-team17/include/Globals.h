#pragma once
#include "Exception.h"

typedef unsigned char byte;

enum ColorEnum { GRAYSCALE, RED, GREEN, BLUE, INVERTED, FULLCOLOUR };
enum filterTypeEnum { MIN, MAX, MEDIAN };
enum noiseTypeEnum { SALTANDPEPPER, GAUSSIANNOISE };
enum interpolationEnum { ZERO_ORDER, FIRST_ORDER };

struct Pixel
{
	Pixel(){}
	Pixel(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a){}
	byte r, g, b, a;
}; //alpha might come in handy later

struct BoundingBox
{
	BoundingBox(){}
	BoundingBox(int minX, int maxX, int minY, int maxY) : minX(minX), maxX(maxX), minY(minY), maxY(maxY){}
	int minX, maxX, minY, maxY;
	int getWidth() { return maxX - minX; }
	int getHeight() { return maxY - minY; }
};
