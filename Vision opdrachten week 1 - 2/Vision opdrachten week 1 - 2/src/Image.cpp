#include "Image.h"

//still need to write copy constrctr ( might improve performance over reading the complete image again and again )
//assignmentoperator - Rule of three?

Image::Image() :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	bins(0),
	imageData(0),
	histogramBins(0)
{}

//copy constructor
Image::Image(const Image& image) :
	filename(""),
	imageWidth(0),
	imageHeight(0),
	bins(0),
	imageData(0),
	histogramBins(0)
{
    filename = image.filename;
    imageWidth = image.imageWidth;
    imageHeight = image.imageHeight;
	bins = image.bins;

    if(image.imageData)
	{
        imageData = new Pixel[imageWidth * imageHeight];
		memcpy(imageData, image.imageData, (imageWidth * imageHeight) * sizeof(Pixel)); // copy them image memories
	}
    if(image.histogramBins)
	{
        histogramBins = new int[bins];
		memcpy(histogramBins, image.histogramBins, bins * sizeof(int)); // copy them memories
	}
}

Image::Image(std::string filename) :
	filename(filename),
	histogramBins(0) // make sure we're not pointing at anything
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
	//delete [] histogramBins;
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

	//corona::Image* saveImage = corona::CreateImage(imageWidth, imageHeight, corona::PF_R8G8B8, data);
	//delete [] data; //free up them mems
	//return corona::SaveImage(filename.c_str(), corona::FF_PNG, saveImage);
	/*
	* Do we have to write our own file export?
	* If so, convert to BMP
	* Still need to hack all the details tho
	*/

	//Create a new file for writing
	FILE *pFile = fopen(filename.c_str(), "wb");

	if(pFile == NULL)
	{ 
		return false;
	}

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
	return true; 
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

	case INVERTED://added just for fun
		for (int i = 0; i < imageWidth * imageHeight; ++i) 
		{
			imageData[i].r = (byte)(255 - (int)imageData[i].r);
			imageData[i].g = (byte)(255 - (int)imageData[i].g);
			imageData[i].b = (byte)(255 - (int)imageData[i].b);
		}
		break;
	}
}

bool Image::calculateBins(int bins, OutputColorEnum color) //presuming this is a grayscale image!
{	
	histogramBins = new int[bins](); //allocate new spees -> might be messy tho. 

	for (int i = 0; i < imageWidth * imageHeight; ++i) 
	{
		int val = 0;
		switch(color)
		{
			case RED:
				val = (int)imageData[i].r; 
				break;
			case GREEN:
				val = (int)imageData[i].g;
				break;
			case BLUE:
				val = (int)imageData[i].b;
				break;
			case GRAYSCALE:
				val = (int)(imageData[i].r + imageData[i].g + imageData[i].b) / 3;
				break;
		}
		histogramBins[(int)(floor(((val * bins) / 256) + 0.5))]++;//floor+0.5 for proper rounding
	}
	return true;
}

bool Image::EqualizeImage(int bins)
{
	long imageSize = (imageWidth * imageHeight);
	//scaling factor, x = 255 / number of pixels
	float scale = (float)bins / (float)imageSize;

	int* lookupHistogram = new int[bins]();

	// Build Cumulative Histogram which will be our lookup table 
	lookupHistogram[0] = histogramBins[0] * scale; 
	int sum = 0;
	for (int i = 1; i < bins; i++) { 
		sum += histogramBins[i];
		lookupHistogram[i] = sum * scale;
	}

	//now reassign all pixelss
	for (int i = 0; i < imageSize; i++) 
	{ 
		int newValue = lookupHistogram[(int)imageData[i].r];//doesnt really matter which value we get(rgb), they should be the same.
		if (newValue > 255) 
		{ 
			imageData[i].r = imageData[i].g = imageData[i].b = (int)(255); 
		} 
		else 
		{
			imageData[i].r = imageData[i].g = imageData[i].b = (int)(newValue); 
		}
	} 

	delete [] lookupHistogram; //cleanup
	return true;

}

bool Image::saveHistogramAsCSV(int bins, std::string color)
{
	std::ofstream csvFile;
	std::stringstream ss;
	ss << bins << "_" << color << "_" << getFileNameWithoutExtension() << ".csv";
	csvFile.open(ss.str()); 
	csvFile << "binnr" << "," << "Density"  << std::endl; 

	for (int i = 0 ; i < bins; i++) 
	{ 
		csvFile << i << "," << (int)histogramBins[i]  << std::endl; 
	} 

	csvFile.close(); 
	return true;
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

