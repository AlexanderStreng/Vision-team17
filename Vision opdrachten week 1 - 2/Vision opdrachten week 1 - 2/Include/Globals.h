#pragma once

typedef unsigned char byte;
enum OutputColorEnum { GRAYSCALE, RED, GREEN, BLUE, INVERTED };
struct Pixel
{
	Pixel(){}
	Pixel(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a){}
	byte r, g, b, a;
}; //alpha might come in handy later