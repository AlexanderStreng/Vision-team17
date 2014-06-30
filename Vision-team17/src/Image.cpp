#include "Image.h"

//need assignmentoperator? - Rule of three(but its not needed for now)?
Image::Image() :
	filename(""),
	imageWidth(0),
	imageHeight(0)
{}

//copy constructor
Image::Image(const Image& image) :
	filename(""),
	imageWidth(0),
	imageHeight(0)
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
	if(image.colorData)
	{
		colorData = new Pixel[imageWidth * imageHeight];
		memcpy(colorData, image.colorData, (imageWidth * imageHeight) * sizeof(Pixel)); // copy them image memories
	}
}

Image::Image(std::string filename) :
	filename(filename)
{
	try 
	{
		inputImage = corona::OpenImage(filename.c_str(), corona::PF_R8G8B8); //Open the image using Corona lib
	}
	catch (const std::exception& e) 
	{
		std::cout << e.what() << '\n';
	}

	imageWidth  = inputImage->getWidth();
	imageHeight = inputImage->getHeight();


	int imageSize = imageWidth*imageHeight;

	//allocate pixel buffers
	grayData = new byte[imageSize];
	redData = new byte[imageSize];
	greenData = new byte[imageSize];
	blueData = new byte[imageSize];
	colorData = new Pixel[imageSize];

	void* pixels = inputImage->getPixels();
	byte* p = (byte*)pixels;

	//(from the lib.. )
	// we're guaranteed that the first eight bits of every pixel is red
	byte r = 0, g = 0, b = 0;
	for (int i = 0; i < imageWidth * imageHeight; ++i) //copy all the data into our buffer
	{
		r = *p++;
		redData[i] = r;

		g = *p++;
		greenData[i] = g;

		b = *p++;
		blueData[i] = b;

		grayData[i] = ((r + g + b) / 3);	
		colorData[i] = Pixel(r, g, b, 0);
	}

	delete inputImage; //cleanup
}

Image::Image(int width, int height, std::string fileName) :
	imageWidth(width),
	imageHeight(height),
	filename(fileName)
{
	int imageSize = imageWidth*imageHeight;

	//allocate pixel buffers
	grayData = new byte[imageSize];
	redData = new byte[imageSize];
	greenData = new byte[imageSize];
	blueData = new byte[imageSize];
	colorData = new Pixel[imageSize];
}

Image::~Image()
{
	delete [] colorData;
	delete [] blueData;
	delete [] greenData;
	delete [] redData;
	delete [] grayData;
}

void Image::setImagePixelData(Pixel* imageData)
{
	colorData = imageData;
}

void Image::setImageData(ColorEnum color, byte* imageData)
{
	switch(color) {
	case GRAYSCALE:
		grayData = imageData;
		break;
	case RED:
		redData = imageData;
		break;
	case GREEN:
		greenData = imageData;
		break;
	case BLUE:
		blueData = imageData;
		break;
	}
}

bool Image::saveToFile(std::string filename, ColorEnum color)
{
	int imageSize = imageWidth * imageHeight;
	byte* data = new byte[imageSize * 3];

	switch(color) { //might be more efficient to switch outside the loop.
	case GRAYSCALE:
		for (int i = 0; i < imageSize; ++i) 
		{
			data[i * 3] = grayData[i];
			data[(i * 3) + 1] = grayData[i];
			data[(i * 3) + 2] = grayData[i];
		}
		break;
	case RED:
		for (int i = 0; i < imageSize; ++i) 
		{
			data[i * 3] = redData[i];
			data[(i * 3) + 1] = 0;
			data[(i * 3) + 2] = 0;
		}
		break;
	case GREEN:
		for (int i = 0; i < imageSize; ++i) 
		{
			data[i * 3] = 0;
			data[(i * 3) + 1] = greenData[i];
			data[(i * 3) + 2] = 0;
		}
		break;
	case BLUE:
		for (int i = 0; i < imageSize; ++i) 
		{
			data[i * 3] = 0;
			data[(i * 3) + 1] = 0;
			data[(i * 3) + 2] = blueData[i];
		}
		break;	
	case FULLCOLOUR:
		for (int i = 0; i < imageSize; ++i) 
		{
			data[i * 3] = colorData[i].r;
			data[(i * 3) + 1] = colorData[i].g;
			data[(i * 3) + 2] = colorData[i].b;
		}
		break;
	default:
		for (int i = 0; i < imageSize; ++i) 
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
		int imageSize = imageWidth * imageHeight;
		for (int i = 0; i < imageSize; ++i) 
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
			colorData[i] = Pixel(val, val, val, 0);
			bitsFlipped++;
		}
	}

	return bitsFlipped;
}

double Image::compareToImage(Image* otherImage, ColorEnum color)
{
	if(otherImage->getHeight() != imageHeight ||
		otherImage->getWidth() != imageWidth) {
			return -1;
	}

	int imageSize = imageWidth * imageHeight;
	double currentDistance = 0;
	double totaldiff = 0.0 ; //holds the number of different pixels

	switch(color)
	{
	case FULLCOLOUR:
		{
			Pixel* otherColorData = otherImage->getImagePixelData();
			for (int i = 0; i < imageSize; ++i) 
			{
				int red = (int)colorData[i].r, green = (int)colorData[i].g, blue = (int)colorData[i].b;
				int otherRed = (int)otherColorData[i].r, otherGreen = (int)otherColorData[i].g, otherBlue = (int)otherColorData[i].b;

				totaldiff += std::abs(otherRed-red) / 255.0 ;
				totaldiff += std::abs(otherGreen-green) / 255.0 ;
				totaldiff += std::abs(otherBlue-blue) / 255.0 ;

				double distance = sqrt(pow((otherRed - red), 2) + pow((otherGreen - green), 2) + pow((otherBlue - blue), 2)); 
				currentDistance += distance;
			}
			return (totaldiff * 100)  / (imageSize * 3);
		}
		break;
	case GRAYSCALE:
		{
			byte* otherGrayData = otherImage->getImageData(GRAYSCALE);
			for (int i = 0; i < imageSize; ++i) 
			{
				int gray = (int)grayData[i];
				int otherGray = (int)otherGrayData[i];

				totaldiff += std::abs( gray - otherGray ) / 255.0 ;

				double distance = sqrt(pow((otherGray - gray), 2)); 
				currentDistance += distance;
			}
			return (totaldiff * 100)  / imageSize;
		}
		break;
	}
	return (currentDistance / imageSize);
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

Pixel* Image::getImagePixelData() {
	return colorData;
}

std::string Image::getFileName()
{
	return filename;
}

std::string Image::getFileNameWithoutExtension()
{
	char splitted_filename[50]; // max file name;
	_splitpath(filename.c_str(), NULL, NULL, splitted_filename, NULL);
	std::string str(splitted_filename);
	//delete [] splitted_filename; // cleanup;
	return str;
}

