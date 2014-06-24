#include "Treshold.h"


Treshold::Treshold(Image *image) : 
	imagePtr(image)
{
	imageWidth = imagePtr->getWidth();
	imageHeight = imagePtr->getHeight();
	imageGrayData = imagePtr->getImageData(GRAYSCALE); //pointer to our data. ( we need pixels. ( grayscale ) )
}


Treshold::~Treshold(void)
{
}


int Treshold::determineTreshHold()
{
	int u1Pixels = (imageWidth * 2) + (imageHeight * 2); // cornerPixels
	int u2Pixels = (imageWidth * imageHeight) - u1Pixels;
	int sumCorner = 0, sumRest = 0;

	//calculate mean grey level of corner pixels & calculate mean grey level of all other pixels
	for(int x = 0; x < imageWidth; x++)
	{
		for(int y = 0; y < imageHeight; y++)
		{
			if(x == 0 || y == 0 || x == (imageWidth - 1) || y == (imageHeight - 1)) //its a corner
			{
				sumCorner += (int)imageGrayData[ (y * imageWidth) + x ];
			} 
			else 
			{
				sumRest += (int)imageGrayData[ (y * imageWidth) + x ];
			}

		}
	}

	int u1 = sumCorner / u1Pixels;
	int u2 = sumRest / u2Pixels;

	int tOld = 0;
	int tNew = (u1 + u2) / 2;

	while(tNew != tOld)
	{
		u1 = u2 = u1Pixels = u2Pixels = 0;

		for(int index = 0; index < (imageWidth * imageHeight); index++)
		{
			int value = (int)imageGrayData[index];
			if(value < tNew)
			{
				u1 += value;
				u1Pixels++;
			}
			else 
			{
				u2 += value;
				u2Pixels++;
			}
		}

		u1 = u1 / u1Pixels;
		u2 = u2 / u2Pixels;

		tOld = tNew;
		tNew = (u1 + u2) / 2;
	}
	return tNew;
}

void Treshold::doTreshHold(int treshHold)
{
	for(int index = 0; index < (imageWidth * imageHeight); index++)
		{
			int value = (int)imageGrayData[ index ];
			if(value >= treshHold)
			{
				imageGrayData[index] = (byte)255;
			}
			else 
			{
				imageGrayData[index] = (byte)0;
			}
		}
}