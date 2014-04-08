#include "Kmeans.h"

Kmeans::Kmeans(int amountOfMeans, ColorEnum color, Image * image) : 
	amountOfMeans(amountOfMeans),
	imagePtr(image)
{
	imageWidth = imagePtr->getWidth();
	imageHeight = imagePtr->getHeight();
	imageData = imagePtr->getPixelData(); //pointer to our data. ( we need pixels. ( RGB ) )
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
		iterations++;
		//foreach pixel in image
		for (int i = 0; i < imageWidth * imageHeight; ++i) {
			// calculate distance to (random)mean
			Mean closestMean = meansArray[0];
			int smallestDist = INT_MAX, red = (int)imageData[i].r, green = (int)imageData[i].g, blue = (int)imageData[i].b;
			for(int j = 0; j < amountOfMeans; j++) {
				int kRed = (int)meansArray[j].centerPixel.r, kGreen = (int)meansArray[j].centerPixel.g, kBlue = (int)meansArray[j].centerPixel.b;
				int distance = sqrt(((kRed - red)^2) + ((kGreen - green)^2) + ((kBlue - blue)^2)); 
				//determine closest mean
				if(distance < smallestDist) {
					smallestDist = distance;
					closestMean = meansArray[j];
				}
			}
			// label to nearest mean
			closestMean.assignedPixels.push_back(&imageData[i]);
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
		std::cout << " Done " << iterations << " iterations " << std::endl;
	}
	changeClusterColors();
	return iterations;
}

inline bool Kmeans::evaluateCentroidsEqual() {
	bool returnValue = true;
	for(int i = 0; i < amountOfMeans; i++) {

		int distance = sqrt((meansArray[i].centerPixel.r - meansArray[i].lastCenterPixel.r)^2 
			+ (meansArray[i].centerPixel.g - meansArray[i].lastCenterPixel.g)^2
			+ (meansArray[i].centerPixel.b - meansArray[i].lastCenterPixel.b)^2); 

		std::cout << " checked Distance. Distances between prev and current centroid from mean " << meansArray[i].label << " is " <<  distance << std::endl;

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