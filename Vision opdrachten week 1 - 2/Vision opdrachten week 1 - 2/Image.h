#pragma once


class Image
{
	private:
		typedef unsigned char byte;
		int _width;
		int _height;
	public:
		Image::Image();
		Image(int width, int height, void* pixels);
		void setPixels(void* pixels);
		~Image(void);
};

