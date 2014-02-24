#include "Image.h"

//still need to write copy constrctr ( might improve performance over reading the complete image again and again )
//assignmentoperator - Rule of three(but its not needed for now)?
Image::Image() :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	grayData(0),
	redData(0),
	greenData(0),
	blueData(0)
{}

//copy constructor
Image::Image(const Image& image) :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	grayData(0),
	redData(0),
	greenData(0),
	blueData(0)
{
	filename = image.filename;
	imageWidth = image.imageWidth;
	imageHeight = image.imageHeight;

	if(image.grayData)
	{
		grayData = new byte[imageWidth * imageHeight];
		memcpy(grayData, image.grayData, (imageWidth * imageHeight) * sizeof(byte)); // copy them image memories
	}	
	if(image.redData)
	{
		redData = new byte[imageWidth * imageHeight];
		memcpy(redData, image.redData, (imageWidth * imageHeight) * sizeof(byte)); // copy them image memories
	}	
	if(image.greenData)
	{
		greenData = new byte[imageWidth * imageHeight];
		memcpy(greenData, image.greenData, (imageWidth * imageHeight) * sizeof(byte)); // copy them image memories
	}	
	if(image.blueData)
	{
		blueData = new byte[imageWidth * imageHeight];
		memcpy(blueData, image.blueData, (imageWidth * imageHeight) * sizeof(byte)); // copy them image memories
	}
}

Image::Image(std::string filename) :
	filename(filename)
{
	inputImage = corona::OpenImage(filename.c_str(), corona::PF_R8G8B8); //Open the image using Corona lib

	imageWidth  = inputImage->getWidth();
	imageHeight = inputImage->getHeight();

	//allocate pixel buffers
	grayData = new byte[imageWidth * imageHeight];
	redData = new byte[imageWidth * imageHeight];
	greenData = new byte[imageWidth * imageHeight];
	blueData = new byte[imageWidth * imageHeight];

	void* pixels = inputImage->getPixels();
	byte* p = (byte*)pixels;

	//(from the lib.. )
	// we're guaranteed that the first eight bits of every pixel is red
	byte gray;
	for (int i = 0; i < imageWidth * imageHeight; ++i) //copy all the data into our buffer
	{
		redData[i] = *p++;
		greenData[i] = *p++;
		blueData[i] = *p++;
		grayData[i] = ((redData[i] + greenData[i] + blueData[i]) / 3);
	}

	delete inputImage; //cleanup the image in mem
}

Image::~Image()
{
	//delete [] imageData;
}

bool Image::saveToFile(std::string filename, ColorEnum color)
{
	byte* data = new byte[imageWidth * imageHeight * 3];

	switch(color) { //might be more efficient to switch outside the loop.
	case GRAYSCALE:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			data[i * 3] = grayData[i];
			data[(i * 3) + 1] = grayData[i];
			data[(i * 3) + 2] = grayData[i];
		}
		break;
	case RED:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			data[i * 3] = redData[i];
			data[(i * 3) + 1] = 0;
			data[(i * 3) + 2] = 0;
		}
		break;
	case GREEN:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			data[i * 3] = 0;
			data[(i * 3) + 1] = greenData[i];
			data[(i * 3) + 2] = 0;
		}
		break;
	case BLUE:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			data[i * 3] = 0;
			data[(i * 3) + 1] = 0;
			data[(i * 3) + 2] = blueData[i];
		}
		break;	
	default:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			data[i * 3] = redData[i];
			data[(i * 3) + 1] = greenData[i];
			data[(i * 3) + 2] = blueData[i];
		}
		break;
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

void Image::convertToColor(ColorEnum color) //pretty much depricated
{
	switch(color)
	{
	case INVERTED:
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			redData[i] = (byte)(255 - (int)redData[i]);
			greenData[i] = (byte)(255 - (int)greenData[i]);
			blueData[i] = (byte)(255 - (int)blueData[i]);
		}
		break;
	}
}

int Image::addNoise(int amount, noiseTypeEnum noise){ // amount in % // Gaussian noise is not implemented yet.
	int bitsFlipped = 0;
	srand (time(NULL));  // set seed
	for (int i = 0; i < imageWidth * imageHeight; ++i) 
	{
		if((rand() % 100) < amount )
		{
			int val = 0;
			if((rand() % 2) == 1) {
				val = 255;
			} 
			redData[i] = greenData[i] = blueData[i] = grayData[i] = (byte)val; //adds noise to all the channels!
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

byte* Image::getImageData(ColorEnum color){
	switch(color) {
	case GRAYSCALE:
		return grayData;
		break;
	case RED:
		return redData;
		break;
	case GREEN:
		return greenData;
		break;
	case BLUE:
		return blueData;
		break;
	}
}

Pixel* Image::getPixelData() {
	Pixel* pixelData = new Pixel[imageWidth * imageHeight];

	for (int i = 0; i < imageWidth * imageHeight; ++i) //copy all the data into our buffer
	{
		redData[i] = pixelData[i].r;
		greenData[i] = pixelData[i].g;
		blueData[i] = pixelData[i].b;
	}
	return pixelData;
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

