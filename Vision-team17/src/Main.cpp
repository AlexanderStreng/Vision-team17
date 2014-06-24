#include <iostream>
#include "Image.h"
#include "Histogram.h"
#include "Filter.h"
#include "TransformationMatrix.h"
#include "exectimer.h"
#include "basetimer.h"
#include "Globals.h"
#include "Kmeans.h"
#include "AffineTransform.h"
#include "Treshold.h"
#include <random>
#include <time.h>       /* time */

std::string filename, yesOrNo;
Image originalImage;
std::stringstream ss;
BaseTimer* bt;

bool exerciseRoutine();
void grayScaleRoutine(); //Preventing clutter in main function
void colorRoutine();
void invertRoutine();
void FilterRoutine();
void KmeansRoutine();
void affineTransformRoutine();
void stop(std::string msg);

int main(int argc, char* argv[])
{
	//check argv[1] for input, if not, ask for input.
	if(argc < 2)
	{
		std::cout << std::endl << "No filename found. Could you please insert the filename? " << std::endl;
		getline(std::cin, filename);
	} 
	else
	{		
		std::cout << "Command line argument found." << std::endl;
		filename = argv[1];
	}

	std::cout << "I am going to use the file: '" << filename << "'" << std::endl;

	bt = new BaseTimer(); //lets create a fancy microsecond timer.

	bt->start();
	originalImage = Image(filename);
	bt->stop(); bt->store("Load_Image");

	if (!originalImage.Exists()) {
		stop("Image could not be loaded");
	}

	std::cout << "Loaded img:" << originalImage.getFileNameWithoutExtension() << " dimensions(WxH):"  
		<< originalImage.getWidth() << " x " << originalImage.getHeight() << " \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl << std::endl;

	bool chosen = false;
	do {
		chosen = exerciseRoutine();
	} while(!chosen);

	ss.str("");
	ss << "Timings_" << originalImage.getFileNameWithoutExtension() << ".csv"; // save as png
	bt->save(ss.str());

	stop("");
}

bool exerciseRoutine() {
	std::cout << "Which week's exercises do you want to run?" << std::endl
		<< "Press (1) for week 1&2 ( Histograms, median filtering )" << std::endl
		<< "Press (2) for week 3&4 ( Kmeans clustering )" << std::endl
		<< "Press (3) for week 5&6 ( Affine transformations )" << std::endl;
	char option = getchar();
	switch(option) {
	case '1':
		std::cout << "You have chosen to run week 1 and 2. Lets go!" << std::endl;
		grayScaleRoutine();
		colorRoutine();
		invertRoutine();
		FilterRoutine();
		return true;
	case '2':
		std::cout << "You have chosen to run week 3 and 4. Lets go!" << std::endl;
		KmeansRoutine();
		return true;
	case '3':
		std::cout << "You have chosen to run week 5 and 6. Lets go!" << std::endl;
		affineTransformRoutine();
		return true;
	default:
		std::cout << "Input could not be read." << std::endl;
		return false;
	}
}

void affineTransformRoutine(){
	//prepare image
	bt->reset(); bt->start();
	Image sourceImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	std::cout << std::endl << "We need a txt file containing the transformationMatrix. Could you please insert the filename? " << std::endl;
	std::string transformMatrixFileName;
	std::cin.ignore();
	getline(std::cin, transformMatrixFileName);
	TransformationMatrix transformMatrix = TransformationMatrix(transformMatrixFileName);


	std::cout << std::endl << "Next we need to determine which interpolation we want to use. Press (0) for zero-order and (1) for first order. " << std::endl;
	char option = getchar();
	AffineTransform transform = AffineTransform(FULLCOLOUR, &sourceImage, &transformMatrix);

	if(option == '0') 
	{
		std::cout << "Lets go(0)!" << std::endl;
		transform.setInterpolationMethod(ZERO_ORDER);
	} 
	else if(option == '1') 
	{
		std::cout << "Lets go(1)!" << std::endl;
		transform.setInterpolationMethod(FIRST_ORDER);
	} 
	else 
	{
		std::cout << "Wrong input." << std::endl;
	}

	bt->reset(); bt->start();	
	//do transformation
	Image transformedImage = transform.doTransformation();
	bt->stop(); bt->store("Do_Transform");

	std::cout << "Done transformation. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "transformed_" << transformedImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	transformedImage.saveToFile(ss.str(), FULLCOLOUR);
	bt->stop();	bt->store("Save_transformed");
}

void KmeansRoutine()
{
	//get input ( aantal means )

	//prepare image
	bt->reset(); bt->start();
	Image kmeansImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	bt->reset(); bt->start();	
	//do means routine
	Kmeans kmeansSegmentation = Kmeans(3, FULLCOLOUR, &kmeansImage);
	int amountOfIterations = kmeansSegmentation.doMeans();
	bt->stop(); bt->store("Do_Kmeans");

	std::cout << "Done kmeans in " << amountOfIterations << " iterations \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "kmeans_" << kmeansImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	kmeansImage.saveToFile(ss.str(), FULLCOLOUR);
	bt->stop();	bt->store("Save_Kmeans");


		//prepare image
	bt->reset(); bt->start();
	Image thresholdImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	bt->reset(); bt->start();	
	//do means routine
	Treshold treshold = Treshold(&thresholdImage);
	int determinedTreshold = treshold.determineTreshHold();
	bt->stop(); bt->store("Determined Treshold");

	std::cout << "Determined graylevel treshold : " << determinedTreshold << " \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	treshold.doTreshHold(determinedTreshold);
	bt->stop(); bt->store("Repainted Treshold");

	std::cout << "Repainted image according to treshold " << " \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "treshold_" << thresholdImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	thresholdImage.saveToFile(ss.str(), GRAYSCALE); //we want to save the graybytes
	bt->stop();	bt->store("Save_Treshold");
}

void grayScaleRoutine()
{
	std::cout << "Converting the image to grayscale." << std::endl;
	bt->reset(); bt->start();
	Image grayScaleImage = Image(originalImage);
	bt->stop(); bt->store("Copy_image");
	ss.str("");
	ss << "grey_" << grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	bool save = grayScaleImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop(); bt->store("Save_Grayscale");

	if(save)
	{
		std::cout<< "Saving image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

		{
			bt->reset();bt->start();
			Histogram greyScale10Histogram = Histogram(10, GRAYSCALE, &grayScaleImage);
			bt->stop(); bt->store("Calc_bin10_gray");

			if(greyScale10Histogram.saveHistogramAsCSV("Grayscale")){
				std::cout<< "Saving grayscale image-histogram(10) succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
			}
		} //call histogram dtor

		{
			bt->reset();bt->start();
			Histogram greyScale256Histogram = Histogram(256, GRAYSCALE, &grayScaleImage);
			bt->stop(); bt->store("Calc_bin256_gray");

			bt->reset(); bt->start();
			greyScale256Histogram.saveHistogramAsCSV("Grayscale");
			bt->stop(); bt->store("SaveCSV_bin256_gray");

			std::cout<< "Saving grayscale image-histogram(256) succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
			bt->reset(); bt->start();
			greyScale256Histogram.EqualizeImage();
			bt->stop(); bt->store("Equalize_image_bin256");

			std::cout << "Equalized image. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
		}
		ss.str("");
		ss << "equalized_" <<grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as png

		bt->reset(); bt->start();
		if( grayScaleImage.saveToFile(ss.str(), GRAYSCALE))
		{
			bt->stop(); bt->store("Save_Equalized");
			std::cout<< "Saving image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
		}
		else 
		{
			stop("Saving image failed.");
		}
	}
}

void invertRoutine()
{
	bt->reset(); bt->start();
	Image invertedImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	bt->reset(); bt->start();
	invertedImage.convertToColor(INVERTED);
	bt->stop();	bt->store("Convert_Inverted");

	std::cout << "Inverting image colors. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "inverted_" <<invertedImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	bool save = invertedImage.saveToFile(ss.str(), INVERTED);
	bt->stop();	bt->store("Save_inverted");

	if(save)
	{
		std::cout<< "Saving image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
	}
	else 
	{
		stop("Saving image failed.");
	}
}

void colorRoutine()
{
	bool save = false;
	{
		ss.str("");
		std::cout << "Converting the image to R(ed) color channel." << std::endl;

		bt->reset(); bt->start();
		Image colorImage = Image(originalImage);
		bt->stop();	bt->store("Copy_originalImage");

		ss << "R_" << colorImage.getFileNameWithoutExtension() << ".png"; // save as png

		bt->reset(); bt->start();
		save = colorImage.saveToFile(ss.str(), RED);
		bt->stop();	bt->store("Save_Red");

		if(save)
		{
			std::cout<< "Saving image RED succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

			bt->reset(); bt->start();
			Histogram red10Histogram = Histogram(10, RED, &colorImage);
			bt->stop();	bt->store("Calc_bin10_red");

			bt->reset(); bt->start();
			red10Histogram.saveHistogramAsCSV("Red");
			bt->stop();	bt->store("SaveCSV_bin256_red");

			std::cout<< "Saving red image-histogram(10) succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
		}

		ss.str("");
		std::cout << "Converting the image to G(reen) color channel." << std::endl;
		ss << "G_" << colorImage.getFileNameWithoutExtension() << ".png"; // save as png

		bt->reset(); bt->start();
		save = colorImage.saveToFile(ss.str(), GREEN);
		bt->stop();	bt->store("Save_Green");

		if(save)
		{
			std::cout<< "Saving image Green succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

			bt->reset(); bt->start();
			Histogram green10Histogram = Histogram(10, GREEN, &colorImage);
			bt->stop();	bt->store("Calc_bin10_green");

			bt->reset(); bt->start();
			green10Histogram.saveHistogramAsCSV("Green");
			bt->stop();	bt->store("SaveCSV_bin256_green");

			std::cout<< "Saving green image-histogram(10) succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
		}

		ss.str("");
		std::cout << "Converting the image to B(lue) color channel." << std::endl;
		ss << "B_" << colorImage.getFileNameWithoutExtension() << ".png"; // save as png
		bt->reset(); bt->start();
		save = colorImage.saveToFile(ss.str(), BLUE);
		bt->stop();	bt->store("Save_Blue");

		if(save)
		{
			std::cout<< "Saving image Blue succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;


			bt->reset(); bt->start();
			Histogram blue10Histogram = Histogram(10, BLUE, &colorImage);
			bt->stop();	bt->store("Calc_bin10_blue");

			bt->reset(); bt->start();
			blue10Histogram.saveHistogramAsCSV("Blue");
			bt->stop();	bt->store("SaveCSV_bin256_blue");

			std::cout<< "Saving blue image-histogram(10) succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;
		} 
		else
		{
			stop("Saving image failed.");
		}
	}
}

void FilterRoutine()
{
	std::cout << "Applying Salt and pepper noise to image." << std::endl;

	bt->reset(); bt->start();
	Image saltAndPepperImage = Image(originalImage);
	bt->stop();	bt->store("Copy_originalImage");

	bt->reset(); bt->start();
	int bitsFlipped = saltAndPepperImage.addNoise(15, SALTANDPEPPER);
	bt->stop();	bt->store("Add_salt_pepper_noise");
	std::cout << "Added noise to image. Percentage converted: " << (((double)bitsFlipped / (originalImage.getWidth() * originalImage.getHeight())) * 100) << "%. \t(in " << bt->elapsedMilliSeconds()  << "		milliseconds)" << std::endl;

	ss.str("");
	ss << "noise_" << saltAndPepperImage.getFileNameWithoutExtension() << ".png"; // save as png
	bt->reset(); bt->start();
	saltAndPepperImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("Save_saltAndPepperImage");
	std::cout<< "Saving noise image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	Image MedianImage = Image(saltAndPepperImage);
	bt->reset(); bt->start();
	Filter medianFilter = Filter(&MedianImage, 3, GRAYSCALE);
	medianFilter.ApplyFilter(MEDIAN, 1);
	bt->stop();	bt->store("Did_medianFilter");
	std::cout<< "Median filter succeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "median_" << MedianImage.getFileNameWithoutExtension() << ".png"; // save as png
	bt->reset(); bt->start();
	MedianImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("Save_medianImage");
	std::cout<< "Saving median filtered image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	Image MinImage = Image(saltAndPepperImage);
	bt->reset(); bt->start();
	Filter minFilter = Filter(&MinImage, 3, GRAYSCALE);
	minFilter.ApplyFilter(MIN, 1);
	bt->stop();	bt->store("Did_minFilter");
	std::cout<< "Min filter succeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "min_" << MinImage.getFileNameWithoutExtension() << ".png"; // save as png
	bt->reset(); bt->start();
	MinImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("Save_minImage");
	std::cout<< "Saving min filtered image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	Image MaxImage = Image(saltAndPepperImage);
	bt->reset(); bt->start();
	Filter maxFilter = Filter(&MaxImage, 3, GRAYSCALE);
	maxFilter.ApplyFilter(MAX, 1);
	bt->stop();	bt->store("Did_maxFilter");
	std::cout<< "Max filter succeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "max_" << MaxImage.getFileNameWithoutExtension() << ".png"; // save as png
	bt->reset(); bt->start();
	MaxImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("Save_sMaxImage");
	std::cout<< "Saving max filtered image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

}

void stop(std::string msg)
{
	std::cin.ignore();
	std::cout << msg << std::endl << "Press enter to stop.." << std::endl;
	std::cin.get();
	exit(0);
}