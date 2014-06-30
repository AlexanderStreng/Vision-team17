#include <iostream>
#include "Image.h"
#include "Histogram.h"
#include "Filter.h"
#include "exectimer.h"
#include "basetimer.h"
#include "Globals.h"
#include <random>
#include <time.h>       /* time */

std::string filename;
Image originalImage;
std::stringstream ss;
BaseTimer* bt;

void grayScaleRoutine(); //Preventing clutter in main function
void colorRoutine();
void invertRoutine();
void FilterRoutine();
void MeasurementsRoutine1();
void MeasurementsRoutine2();

int main(int argc, char* argv[])
{
	//check argv[1] for input, if not, ask for input.
	if(argc < 2)
	{
		std::cout << std::endl << "No filename found. Could you please insert the (path+)filename? " << std::endl;
		getline(std::cin, filename);
	} 
	else
	{		
		std::cout << "Command line argument found." << std::endl;
		filename = argv[1];
	}

	std::cout << "I am going to use the file: '" << filename << "'" << std::endl;
	bt = new BaseTimer(); //lets create a (fancy :p) microsecond timer.
	bt->start();
	originalImage = Image(filename);
	bt->stop(); bt->store("Load_Image");
	std::cout << "Loaded img:" << originalImage.getFileNameWithoutExtension() << " dimensions(WxH):"  
		<< originalImage.getWidth() << " x " << originalImage.getHeight() << " \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl << std::endl;


	std::cout << "Which exercise do you want to run?" << std::endl;
	bool chosen = false;
	while(true)
	{
		std::cout << "Press (1) for the first assignment ( Histograms and color channels )" << std::endl
			<< "Press (2) for the second assignment ( Median filtering and noise )" << std::endl
			<< "Press (0) to quit this application." << std::endl << std::endl;

		char option = getchar();
		switch(option) {
		case '1':
			std::cout << "You have chosen Histograms and color channels!" << std::endl;
			grayScaleRoutine();
			colorRoutine();
			std::cout << std::endl << std::endl; //Make the output a bit more readable.
			break;
		case '2':
			std::cout << "You have chosen Median filtering and noise!" << std::endl;
			FilterRoutine();
			std::cout << std::endl << std::endl;
			break;
		case '0':
			ss.str("");
			ss << "Timings_" << originalImage.getFileNameWithoutExtension() << ".csv"; // save as png
			bt->save(ss.str());
			exit(0);
			break;
		default:
			std::cout << "Input is invalid or could not be read." << std::endl;
			chosen = false;
			break;
		}
		while(getchar()!='\n'); // clear input buffer to prevent from a screen showing twice.
	};
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
			throw Exception("Saving image failed.", __LINE__);
		}
	}
}

//Not used anymore.. (depricated)
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
		throw Exception("Saving image failed.", __LINE__);
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
			throw Exception("Saving image failed.", __LINE__);
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
	int bitsFlipped = saltAndPepperImage.addNoise(5, SALTANDPEPPER);
	bt->stop();	bt->store("Add_salt_pepper_noise");
	std::cout << "Added 5% noise to image. Percentage converted: " << (((double)bitsFlipped / (originalImage.getWidth() * originalImage.getHeight())) * 100) << "%. \t(in " << bt->elapsedMilliSeconds()  << "		milliseconds)" << std::endl;

	ss.str("");
	ss << "noise_" << saltAndPepperImage.getFileNameWithoutExtension() << ".png"; // save as png
	bt->reset(); bt->start();
	saltAndPepperImage.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("Save_saltAndPepperImage");
	std::cout<< "Saving noise image succeeded. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	{
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
	}

	{
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
	}

	{
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
}

//Meetrapport routines
void MeasurementsRoutine1() //filter volgorde
{
	bt = new BaseTimer();
	Image originalImage = Image("cat-filters.jpg");

	bt->reset(); bt->start();
	Image minMaxMedian = Image(originalImage);
	minMaxMedian.addNoise(5, SALTANDPEPPER);
	Filter minMaxMedianFilter1 = Filter(&minMaxMedian, 3, GRAYSCALE);
	minMaxMedianFilter1.ApplyFilter(MIN, 1);
	Filter minMaxMedianFilter2 = Filter(&minMaxMedian, 3, GRAYSCALE);
	minMaxMedianFilter2.ApplyFilter(MAX, 1);
	Filter minMaxMedianFilter3 = Filter(&minMaxMedian, 3, GRAYSCALE);
	minMaxMedianFilter3.ApplyFilter(MEDIAN, 1);
	ss.str("");
	ss << "minMaxMedian-distance-to-original_" << originalImage.compareToImage(&minMaxMedian, GRAYSCALE) << "%.png"; // save as png
	minMaxMedian.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("minMaxMedian");
	std::cout<< "Done minMaxMedian \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	Image minMedianMax = Image(originalImage);
	minMedianMax.addNoise(5, SALTANDPEPPER);
	Filter minMedianMaxFilter1 = Filter(&minMedianMax, 3, GRAYSCALE);
	minMedianMaxFilter1.ApplyFilter(MIN, 1);
	Filter minMedianMaxFilter2 = Filter(&minMedianMax, 3, GRAYSCALE);
	minMedianMaxFilter2.ApplyFilter(MEDIAN, 1);
	Filter minMedianMaxFilter3 = Filter(&minMedianMax, 3, GRAYSCALE);
	minMedianMaxFilter3.ApplyFilter(MAX, 1);
	ss.str("");
	ss << "minMedianMax-distance-to-original_" << originalImage.compareToImage(&minMedianMax, GRAYSCALE) << "%.png"; // save as png
	minMedianMax.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("minMedianMax");
	std::cout<< "Done minMedianMax \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	Image maxMinMedian = Image(originalImage);
	maxMinMedian.addNoise(5, SALTANDPEPPER);
	Filter maxMinMedianFilter1 = Filter(&maxMinMedian, 3, GRAYSCALE);
	maxMinMedianFilter1.ApplyFilter(MAX, 1);
	Filter maxMinMedianFilter2 = Filter(&maxMinMedian, 3, GRAYSCALE);
	maxMinMedianFilter2.ApplyFilter(MIN, 1);
	Filter maxMinMedianFilter3 = Filter(&maxMinMedian, 3, GRAYSCALE);
	maxMinMedianFilter3.ApplyFilter(MEDIAN, 1);
	ss.str("");
	ss << "maxMinMedian-distance-to-original_" << originalImage.compareToImage(&maxMinMedian, GRAYSCALE) << "%.png"; // save as png
	maxMinMedian.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("maxMinMedian");
	std::cout<< "Done maxMinMedian \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	Image maxMedianMin = Image(originalImage);
	maxMedianMin.addNoise(5, SALTANDPEPPER);
	Filter maxMedianMinFilter1 = Filter(&maxMedianMin, 3, GRAYSCALE);
	maxMedianMinFilter1.ApplyFilter(MAX, 1);
	Filter maxMedianMinFilter2 = Filter(&maxMedianMin, 3, GRAYSCALE);
	maxMedianMinFilter2.ApplyFilter(MEDIAN, 1);
	Filter maxMedianMinFilter3 = Filter(&maxMedianMin, 3, GRAYSCALE);
	maxMedianMinFilter3.ApplyFilter(MIN, 1);
	ss.str("");
	ss << "maxMedianMin-distance-to-original_" << originalImage.compareToImage(&maxMedianMin, GRAYSCALE) << "%.png"; // save as png
	maxMedianMin.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("maxMedianMin");
	std::cout<< "Done maxMedianMin \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	Image medianMaxMin = Image(originalImage);
	medianMaxMin.addNoise(5, SALTANDPEPPER);
	Filter medianMaxMinFilter1 = Filter(&medianMaxMin, 3, GRAYSCALE);
	medianMaxMinFilter1.ApplyFilter(MEDIAN, 1);
	Filter medianMaxMinFilter2 = Filter(&medianMaxMin, 3, GRAYSCALE);
	medianMaxMinFilter2.ApplyFilter(MAX, 1);
	Filter medianMaxMinFilter3 = Filter(&medianMaxMin, 3, GRAYSCALE);
	medianMaxMinFilter3.ApplyFilter(MIN, 1);
	ss.str("");
	ss << "medianMaxMin-distance-to-original_" << originalImage.compareToImage(&medianMaxMin, GRAYSCALE) << "%.png"; // save as png
	medianMaxMin.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("medianMaxMin");
	std::cout<< "Done medianMaxMin \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	bt->reset(); bt->start();
	Image medianMinMax = Image(originalImage);
	medianMinMax.addNoise(5, SALTANDPEPPER);
	Filter medianMinMaxFilter1 = Filter(&medianMinMax, 3, GRAYSCALE);
	medianMinMaxFilter1.ApplyFilter(MEDIAN, 1);
	Filter medianMinMaxFilter2 = Filter(&medianMinMax, 3, GRAYSCALE);
	medianMinMaxFilter2.ApplyFilter(MIN, 1);
	Filter medianMinMaxFilter3 = Filter(&medianMinMax, 3, GRAYSCALE);
	medianMinMaxFilter3.ApplyFilter(MAX, 1);
	ss.str("");
	ss << "medianMinMax-distance-to-original_" << originalImage.compareToImage(&medianMinMax, GRAYSCALE) << "%.png"; // save as png
	medianMinMax.saveToFile(ss.str(), GRAYSCALE);
	bt->stop();	bt->store("medianMinMax");
	std::cout<< "Done medianMinMax \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "filterOrder_timings.csv"; // save as png
	bt->save(ss.str());
}

//Meetrapport routines
void MeasurementsRoutine2() //histogram equalisation
{
	{
		Image normalCatImage = Image("normal-cat.jpg");
		Histogram normalHistogram = Histogram(256, GRAYSCALE, &normalCatImage);
		normalHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_normal.png";
		normalCatImage.saveToFile(ss.str(), GRAYSCALE);

		Image normalNoiseCatImage = Image("normal-cat.jpg");
		normalNoiseCatImage.addNoise(20, SALTANDPEPPER);
		Histogram normalNoiseHistogram = Histogram(256, GRAYSCALE, &normalNoiseCatImage);
		normalNoiseHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_normal_noise_diff;" << normalNoiseCatImage.compareToImage(&normalCatImage, GRAYSCALE) << "%.png";
		normalNoiseCatImage.saveToFile(ss.str(), GRAYSCALE);
	}

	{
		Image overexposedCatImage = Image("overexposed-cat.jpg");
		Histogram overHistogram = Histogram(256, GRAYSCALE, &overexposedCatImage);
		overHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_over.png";
		overexposedCatImage.saveToFile(ss.str(), GRAYSCALE);

		Image overexposedNoiseCatImage = Image("overexposed-cat.jpg");
		overexposedNoiseCatImage.addNoise(20, SALTANDPEPPER);
		Histogram overNoiseHistogram = Histogram(256, GRAYSCALE, &overexposedNoiseCatImage);
		overNoiseHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_over_noise_diff;" << overexposedNoiseCatImage.compareToImage(&overexposedCatImage, GRAYSCALE) << "%.png";
		overexposedNoiseCatImage.saveToFile(ss.str(), GRAYSCALE);
	}

	{
		Image underexposedCatImage = Image("underexposed-cat.jpg");
		Histogram underHistogram = Histogram(256, GRAYSCALE, &underexposedCatImage);
		underHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_under.png";
		underexposedCatImage.saveToFile(ss.str(), GRAYSCALE);

		Image underexposedNoiseCatImage = Image("underexposed-cat.jpg");
		underexposedNoiseCatImage.addNoise(20, SALTANDPEPPER);
		Histogram underNoiseHistogram = Histogram(256, GRAYSCALE, &underexposedNoiseCatImage);
		underNoiseHistogram.EqualizeImage();
		ss.str(""); ss << "equalized_under_noise_diff;" << underexposedNoiseCatImage.compareToImage(&underexposedCatImage, GRAYSCALE) << "%.png";
		underexposedNoiseCatImage.saveToFile(ss.str(), GRAYSCALE);
	}
}