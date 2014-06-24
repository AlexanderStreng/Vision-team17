#pragma once
#include <iostream> //need std::cout
#include "globals.h"
#include "Image.h"
#include "Mean.h"

class Kmeans
{
	public:
		Kmeans(int amountOfMeans, ColorEnum color, Image * image);
		~Kmeans();
		int doMeans();

	private:
		void createMeans(int means);
		int imageWidth;
		int imageHeight;
		int amountOfMeans;
		Image* imagePtr;
		Pixel* imageData;
		inline bool evaluateCentroidsEqual();
		inline void changeClusterColors();
		std::vector<Mean> meansArray;
};