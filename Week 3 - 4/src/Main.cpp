#include <iostream>
#include "Image.h"
#include "exectimer.h"
#include "basetimer.h"
#include "Globals.h"
#include "Kmeans.h"
#include "Treshold.h"
#include <random>
#include <time.h>       /* time */

std::string filename;
Image originalImage;
std::stringstream ss;
BaseTimer* bt;
int means = 0;

void KmeansRoutine(int means);
void TresholdRoutine();
void MeasurementsRoutine1();
void MeasurementsRoutine2();

int main(int argc, char* argv[])
{
	//check argv[1] for input, if not, ask for input.
	if(argc < 2)
	{
		std::cout << std::endl << "No filename found. Could you please insert the (path+)filename? " << std::endl;
		getline(std::cin, filename);
		std::cout << std::endl << "No amount of means found. Could you please insert the amount of means? " << std::endl;
		std::string meansString;
		getline(std::cin, meansString);
		means = atoi(meansString.c_str());
	} 
	else
	{		
		std::cout << "Command line arguments found." << std::endl;
		filename = argv[1];
		means = atoi(argv[2]);
	}
	std::cout << "I am going to use the file: '" << filename << "' with " << means << " means." << std::endl;

	bt = new BaseTimer(); //lets create a (fancy :p) microsecond timer.
	bt->start();
	originalImage = Image(filename);
	bt->stop(); bt->store("Load_Image");
	std::cout << "Loaded img:" << originalImage.getFileNameWithoutExtension() << " dimensions(WxH):"  
		<< originalImage.getWidth() << " x " << originalImage.getHeight() << " \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl << std::endl;


	std::cout << "" << std::endl;
	bool chosen = false;
	while(true)
	{
		std::cout << "Press (1) to run this application with the selected amount of means (" << means << ")." << std::endl
			<< "Press (2) to run the automatic tresholding algorithm." << std::endl
			<< "Press (0) to quit this application." << std::endl << std::endl;

		char newMeans;
		char option = getchar();
		switch(option) {
		case '1':
			std::cout << "You have chosen to run the Kmeans algorithm. Please be patient! ( an 1920x1080 image can take up to 40 seconds )" << std::endl;
			KmeansRoutine(means);
			std::cout << std::endl << std::endl; //Make the output a bit more readable.
			break;
		case '2':
			std::cout << "You have chosen to run the automatic tresholding algorithm!" << std::endl;
			TresholdRoutine();
			std::cout << std::endl << std::endl; //Make the output a bit more readable.
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

void KmeansRoutine(int means)
{
	//get input ( aantal means )

	//prepare image
	bt->reset(); bt->start();
	Image kmeansImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	bt->reset(); bt->start();	
	//do means routine
	Kmeans kmeansSegmentation = Kmeans(means, FULLCOLOUR, &kmeansImage);
	int amountOfIterations = kmeansSegmentation.doMeans();
	bt->stop(); bt->store("Do_Kmeans");

	std::cout << "Done kmeans in " << amountOfIterations << " iterations \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "kmeans_" << kmeansImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	kmeansImage.saveToFile(ss.str(), FULLCOLOUR);
	bt->stop();	bt->store("Save_Kmeans");
}

void TresholdRoutine()
{
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

//Meetrapport routines
void MeasurementsRoutine1()  //kmeans meetrapport
{
	bt = new BaseTimer();
	Image detailImage = Image("kmeans-detail.jpg");
	Image nodetailImage = Image("kmeans-nodetail.jpg");

	int maxMeans = sqrt((detailImage.getHeight() * detailImage.getWidth() / 2)); //sqrt(n/2) (en floor(x) -> door int)

	std::cout << "Doing " << maxMeans << " total" << std::endl;

	for(int i = 0; i < maxMeans; i++)
	{

		// provide scope, just to be sure dtors are called
		{ 
			bt->reset(); bt->start();
			Image kmeansdetail = Image(detailImage);
			Image kmeansnodetail = Image(nodetailImage);

			//do means routine
			Kmeans kmeansDetailSegmentation = Kmeans(i, FULLCOLOUR, &kmeansdetail);
			Kmeans kmeansNoDetailSegmentation = Kmeans(i, FULLCOLOUR, &kmeansnodetail);

			int amountOfDetailIterations = kmeansDetailSegmentation.doMeans();
			int amountOfNoDetailIterations = kmeansNoDetailSegmentation.doMeans();

			ss.str("");
			ss << "kmeans-detail-iterations_" << amountOfDetailIterations << "_means_" << i << ".png"; // save as png
			kmeansdetail.saveToFile(ss.str(), FULLCOLOUR);

			ss.str("");
			ss << "kmeans-nodetail-iterations_" << amountOfNoDetailIterations << "_means_" << i << ".png"; // save as png
			kmeansnodetail.saveToFile(ss.str(), FULLCOLOUR);

			ss.str("");
			ss << "iteration_" << i;
			bt->stop();	bt->store(ss.str());

			std::cout << "Did iteration " << i << " in " << bt->elapsedMilliSeconds() << std::endl;
		}

	}
	ss.str("");
	ss << "Kmeans_timings.csv"; // save as png
	bt->save(ss.str());
}
//Meetrapport routines
void MeasurementsRoutine2() // treshold meetrapport
{
	bt = new BaseTimer();
	Image tresholdOriginalImage = Image("treshold-dice.jpg");

	for(int i = 0; i <= 100; i++) //we want from 0 - 100
	{
		if(i % 5 == 0)
		{
			{
				bt->reset(); bt->start();
				Image tresholdImage = Image(tresholdOriginalImage);
				tresholdImage.addNoise(i, SALTANDPEPPER);

				//do means routine
				Treshold treshold = Treshold(&tresholdImage);
				int determinedTreshold = treshold.determineTreshHold();
				treshold.doTreshHold(determinedTreshold);

				ss.str("");
				ss << "treshold_amountnoise_" << i << ".png"; // save as png

				tresholdImage.saveToFile(ss.str(), GRAYSCALE); //we want to save the graybytes
				
				ss.str("");
				ss << "treshold_amountnoise:" << i; // save as png
				bt->stop(); bt->store(ss.str());

				std::cout << "Did noise amount " << i << " in " << bt->elapsedMilliSeconds() << std::endl;
			}
		}
	}	
	ss.str("");
	ss << "treshold_timings.csv"; // save as png
	bt->save(ss.str());
}