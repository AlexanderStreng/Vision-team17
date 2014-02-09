#include "Image.h"

Image::Image(){};

Image::Image(std::string filename) :
	_filename(filename)
{
	_inputImage = corona::OpenImage(_filename.c_str(), corona::PF_R8G8B8); //Open the image using Corona lib

	_width  = _inputImage->getWidth();
	_height = _inputImage->getHeight();

	//allocate array
	_imageData = new Pixel[_width * _height];

	pixels = _inputImage->getPixels();
	byte* p = (byte*)pixels;

	//(from the lib.. )
	// we're guaranteed that the first eight bits of every pixel is red
	for (int i = 0; i < _width * _height; ++i) 
	{
		_imageData[i].r = *p++;
		_imageData[i].g = *p++;
		_imageData[i].b = *p++;
	}
}

Image::~Image()
{
	//delete _imageData; <- we should still do this
}

bool Image::saveToFile(std::string filename, OutputColorEnum color)
{
	byte* data = new byte[_width * _height * 3];
	//byte* data1 = new byte[_width * _height * 3];

	for (int i = 0; i < _width * _height; ++i) 
	{
		data[i * 3] = _imageData[i].r;
		data[(i * 3) + 1] = _imageData[i].g;
		data[(i * 3) + 2] = _imageData[i].b;
	}

	//data1 = (byte*)pixels;

	corona::Image* saveImage = corona::CreateImage(_width, _height, corona::PF_R8G8B8, data);
	return corona::SaveImage(filename.c_str(), corona::FF_PNG, saveImage);

	/*
	* Do we have to write our own file export?
	* If so, convert to BMP
	* Still need to hack all the details tho
	

	//Create a new file for writing
	FILE *pFile = fopen(filename.c_str(), "wb");

	if(pFile == NULL)
	{ 
		return false;
	}
	
	BITMAPINFOHEADER BMIH;
	BMIH.biSize = sizeof(BITMAPINFOHEADER);
    BMIH.biWidth = _width;
    BMIH.biHeight = _height;
    BMIH.biPlanes = 1;
    BMIH.biBitCount = 32;
    BMIH.biCompression = 0;
    BMIH.biSizeImage = _width * _height * 3;
    BMIH.biXPelsPerMeter = 0;
    BMIH.biYPelsPerMeter = 0;
    BMIH.biClrUsed = 0;
    BMIH.biClrImportant = 0;

	BITMAPFILEHEADER bmfh;
	int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize; 
	long lImageSize = BMIH.biSizeImage;
	long lFileSize = nBitsOffset + lImageSize;

	bmfh.bfType = 0x4D42;
	bmfh.bfSize = lFileSize;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = nBitsOffset;

	//Write the bitmap file header
	unsigned int nWrittenFileHeaderSize = fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile);
	
	//move file point to the end of file header
    fseek(pFile, sizeof(BITMAPFILEHEADER), SEEK_SET);

	//And then the bitmap info header
	unsigned int nWrittenInfoHeaderSize = fwrite(&BMIH, 1, sizeof(BITMAPINFOHEADER), pFile);

	//move file point to the begging of bitmap data
    fseek(pFile, nBitsOffset, SEEK_SET);

 	unsigned int nWrittenDIBDataSize = 	fwrite(pixels, 1, lImageSize, pFile); //(void*)&_imageData
	fclose(pFile);
	return true; */
}

void Image::convertToGrayScale()
{
	//nothing yet
}

bool Image::Excists()
{
	return _inputImage;
}

int Image::getWidth(){
	return _width;
}

int Image::getHeight(){
	return _height;
}

std::string Image::getFileName()
{
	return _filename;
}

std::string Image::getFileNameWithoutExtension()
{
	std::stringstream ss(_filename);
	std::string item;

	std::getline(ss, item, '.');

	return item;
}

