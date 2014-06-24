#include "Kmeans.h"

Kmeans::Kmeans(int amountOfMeans, ColorEnum color, Image * image) : 
	amountOfMeans(amountOfMeans),
	imagePtr(image)
{
	imageWidth = imagePtr->getWidth();
	imageHeight = imagePtr->getHeight();
	imageData = imagePtr->getImagePixelData(); //pointer to our data. ( we need pixels. ( RGB ) )
}

Kmeans::~Kmeans()
{
}

void Kmeans::createMeans(int means){
	//srand (time(NULL));  // set seed

	for(int i = 0; i < means; i++)
	{
		Pixel pixel = Pixel((byte)(rand() % 255), (byte)(rand() % 255), (byte)(rand() % 255), (byte)0);
		meansArray.push_back(Mean(pixel, i));
	}
}

int Kmeans::doMeans(){

	if(amountOfMeans <= 0) { // Geen means?
		return 0;
	} else {
		createMeans(amountOfMeans); 
	}

	int iterations = 0;
	bool centroidsEqual = false;
	//While means.center != means.center && (iterations < max_iterations)
	while(!centroidsEqual && iterations < 25) {

		for(int i = 0; i < amountOfMeans; i++) {
			meansArray[i].assignedPixels.clear(); //we should empty assignedPixels here
		}
		
		//foreach pixel in image
		for (int i = 0; i < imageWidth * imageHeight; i++) {
			Pixel * p = &imageData[i];

			int smallestDist = INT_MAX, closestMeanIndex = 0, red = (int)p->r, green = (int)p->g, blue = (int)p->b;

			//this might actually work multi-threaded
			for(int j = 0; j < amountOfMeans; j++) {
				int kRed = (int)meansArray[j].centerPixel.r, kGreen = (int)meansArray[j].centerPixel.g, kBlue = (int)meansArray[j].centerPixel.b;
				int distance = sqrt(pow((kRed - red), 2) + pow((kGreen - green), 2) + pow((kBlue - blue), 2)); 
				//determine closest mean
				if(distance < smallestDist) {
					smallestDist = distance;
					closestMeanIndex = j;
				}
			}
			// label to nearest mean
			meansArray[closestMeanIndex].assignedPixels.push_back(p);
		}

		//foreach mean in means
		//calculate mean
		for(int i = 0; i < amountOfMeans; i++) {
			int sumR = 0, sumG = 0, sumB = 0, assignedPixelSize = 0;
			assignedPixelSize = meansArray[i].assignedPixels.size();

			if(assignedPixelSize > 0) {
				for(int j = 0; j < assignedPixelSize; j++) {
					sumR += meansArray[i].assignedPixels[j]->r;
					sumG += meansArray[i].assignedPixels[j]->g;
					sumB += meansArray[i].assignedPixels[j]->b;
				}
				meansArray[i].lastCenterPixel = meansArray[i].centerPixel;
				meansArray[i].centerPixel = Pixel((byte)(sumR / assignedPixelSize), (byte)(sumG / assignedPixelSize), (byte)(sumB / assignedPixelSize), (byte)0);

			} else {
				meansArray[i].lastCenterPixel = meansArray[i].centerPixel; // still need a last centerpixel
			}
		}
		centroidsEqual = evaluateCentroidsEqual();
		iterations++;
	}
	changeClusterColors();//finally pixel cluster colors according to mean
	return iterations;
}

inline bool Kmeans::evaluateCentroidsEqual() {
	bool returnValue = true;
	for(int i = 0; i < amountOfMeans; i++) {

		int distance = 
			sqrt(pow((meansArray[i].centerPixel.r - meansArray[i].lastCenterPixel.r), 2) +
			pow((meansArray[i].centerPixel.g - meansArray[i].lastCenterPixel.g), 2) + 
			pow((meansArray[i].centerPixel.b - meansArray[i].lastCenterPixel.b), 2));

		if(distance != 0) { // should we allow a certain treshold?
			returnValue = false;
		}
	}
	return returnValue;
}

inline void Kmeans::changeClusterColors() {

	for(int i = 0; i < amountOfMeans; i++) {
		int assignedPixelSize = meansArray[i].assignedPixels.size();

		for(int j = 0; j < assignedPixelSize; j++) {
			meansArray[i].assignedPixels[j]->r = meansArray[i].centerPixel.r;
			meansArray[i].assignedPixels[j]->g = meansArray[i].centerPixel.g;
			meansArray[i].assignedPixels[j]->b = meansArray[i].centerPixel.b;
		}
	}

}