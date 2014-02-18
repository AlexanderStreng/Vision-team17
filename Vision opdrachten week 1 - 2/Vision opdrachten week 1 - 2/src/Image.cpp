#include "Image.h"

//still need to write copy constrctr ( might improve performance over reading the complete image again and again )
//assignmentoperator - Rule of three(but its not needed for now)?
Image::Image() :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	imageData(0)
{}

//copy constructor
Image::Image(const Image& image) :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	imageData(0)
{
	filename = image.filename;
	imageWidth = image.imageWidth;
	imageHeight = image.imageHeight;

	if(image.imageData)
	{
		imageData = new Pixel[imageWidth * imageHeight];
		memcpy(imageData, image.imageData, (imageWidth * imageHeight) * sizeof(Pixel)); // copy them image memories
	}
}

Image::Image(std::string filename) :
	filename(filename)
{
	inputImage = corona::OpenImage(filename.c_str(), corona::PF_R8G8B8); //Open the image using Corona lib

	imageWidth  = inputImage->getWidth();
	imageHeight = inputImage->getHeight();

	//allocate pixel buffer
	imageData = new Pixel[imageWidth * imageHeight];

	void* pixels = inputImage->getPixels();
	byte* p = (byte*)pixels;

	//(from the lib.. )
	// we're guaranteed that the first eight bits of every pixel is red
	for (int i = 0; i < imageWidth * imageHeight; ++i) //copy all the data into our buffer
	{
		imageData[i].r = *p++;
		imageData[i].g = *p++;
		imageData[i].b = *p++;
	}

	delete inputImage; //cleanup the image in mem
}

Image::~Image()
{
	//delete [] imageData;
}

bool Image::saveToFile(std::string filename)
{
	byte* data = new byte[imageWidth * imageHeight * 3]; //3 bytes per pixel
	for (int i = 0; i < imageWidth * imageHeight; ++i) 
	{
		data[i * 3] = imageData[i].r;
		data[(i * 3) + 1] = imageData[i].g;
		data[(i * 3) + 2] = imageData[i].b;
	}

	corona::Image* saveImage = corona::CreateImage(imageWidth, imageHeight, corona::PF_R8G8B8, data);
	delete [] data; //free up them mems 
	return corona::SaveImage(filename.c_str(), corona::FF_PNG, saveImage);
	/*
	* Do we have to write our own file export?
	* If so, convert to BMP
	* Still need to hack all the details tho
	*/

	/*//Create a new file for writing
	FILE *pFile = fopen(filename.c_str(), "wb");

	if(pFile == NULL)
	{ 
	return false;
	}
	//still need to write proper haeder
	BITMAPINFOHEADER BMIH;
	BMIH.biSize = sizeof(BITMAPINFOHEADER);
	BMIH.biWidth = imageWidth;
	BMIH.biHeight = imageHeight;
	BMIH.biPlanes = 1;
	BMIH.biBitCount = 32;
	BMIH.biCompression = 0;
	BMIH.biSizeImage = imageWidth * imageHeight * 3 ;
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

	unsigned int nWrittenDIBDataSize = 	fwrite(data, 1, lImageSize, pFile); //(void*)&imageData
	fclose(pFile);
	return true; */
}

void Image::convertToColor(OutputColorEnum color)
{
	switch(color)
	{
	case GRAYSCALE:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			int val = ((int)imageData[i].r + (int)imageData[i].g + (int)imageData[i].b) / 3;
			imageData[i].r = imageData[i].g = imageData[i].b = (byte)val;
		}
		break;

	case RED:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			imageData[i].g = (byte)0; //color em black? or white? what is best?
			imageData[i].b = (byte)0;
		}
		break;

	case GREEN:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			imageData[i].r = (byte)0; //I kinda prefer black
			imageData[i].b = (byte)0;
		}
		break;

	case BLUE:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			imageData[i].g = (byte)0; //Makes the image better defined
			imageData[i].r = (byte)0;
		}
		break;

	case INVERTED:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			imageData[i].r = (byte)(255 - (int)imageData[i].r);
			imageData[i].g = (byte)(255 - (int)imageData[i].g);
			imageData[i].b = (byte)(255 - (int)imageData[i].b);
		}
		break;
	}
}

int Image::addNoise(int amount, noiseTypeEnum noise){ // amount in %
	int bitsFlipped = 0;
	std::mt19937 mt(time(NULL));
	std::uniform_int_distribution<int> noiseProb(0, 99);
	std::uniform_int_distribution<int> blackOrWhite(0, 1);
	for (int i = 0; i < imageWidth * imageHeight; ++i) 
	{
		if(noiseProb(mt) <= amount )
		{
			int val = 0;
			if(blackOrWhite(mt) == 1) {
				val = 255;
			} 
			imageData[i].r = imageData[i].g = imageData[i].b = (byte)val;
			bitsFlipped++;
		}
	}
	return bitsFlipped;
}

bool Image::Exists()
{
	return inputImage;
}

int Image::getWidth(){
	return imageWidth;
}

int Image::getHeight(){
	return imageHeight;
}

Pixel* Image::getImageData(){
	return imageData;
}

std::string Image::getFileName()
{
	return filename;
}

std::string Image::getFileNameWithoutExtension()
{
	std::stringstream ss(filename);
	std::string item;
	std::getline(ss, item, '.');
	return item;
}

