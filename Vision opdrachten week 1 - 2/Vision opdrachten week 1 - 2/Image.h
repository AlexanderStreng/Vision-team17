#pragma once
#include <string>
#include "corona.h"

class Image
{
	private:
		typedef unsigned char byte;
		int width;
		int height;
		std::string _filename;
	public:
		Image::Image();
		Image(std::string filename);
		bool Excists();
		int getWidth();
		int getHeight();
		bool Image::saveToFile(std::string filename);
		void convertToGrayScale();
		~Image(void);
};

