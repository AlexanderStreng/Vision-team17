#include <iostream>
#include "Image.h"
#include "exectimer.h"
#include "basetimer.h"

std::string filename, yesOrNo;
Image originalImage;
std::stringstream ss;
BaseTimer* bt;

void stop(std::string msg); //forward declare this function.
void grayScaleRoutine(); //Preventing clutter in main function
void colorRoutine();
void invertRoutine();

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
		<< originalImage.getWidth() << " x " << originalImage.getHeight() << " (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

	grayScaleRoutine();
	colorRoutine();
	invertRoutine();

	ss.str("");
	ss << "Timings_" << originalImage.getFileNameWithoutExtension() << ".csv"; // save as png
	bt->save(ss.str());
	delete bt;
	stop("");
}

void grayScaleRoutine()
{
	std::cout << "Converting the image to grayscale." << std::endl;
	bt->reset(); bt->start();
	Image grayScaleImage = Image(originalImage);
	bt->stop(); bt->store("Copy_image");

	bt->reset(); bt->start();
	grayScaleImage.convertToColor(Image::GRAYSCALE);
	bt->stop(); bt->store("Convert_Grayscale");

	ss.str("");
	ss << "grey_" <<grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	bool save = grayScaleImage.saveToFile(ss.str());
	bt->stop(); bt->store("Save_Grayscale");

	if(save)
	{
		std::cout<< "Saving image succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

		bt->reset();bt->start();
		bool calculateBin = grayScaleImage.calculateBins(10, Image::GRAYSCALE);
		bt->stop(); bt->store("Calc_bin10_gray");

		if(calculateBin){
			grayScaleImage.saveHistogramAsCSV(10, "Grayscale");
			std::cout<< "Saving grayscale image-histogram(10) succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
		}

		bt->reset();bt->start();
		calculateBin = grayScaleImage.calculateBins(10, Image::GRAYSCALE);
		bt->stop(); bt->store("Calc_bin256_gray");

		if(calculateBin){

			bt->reset(); bt->start();
			grayScaleImage.saveHistogramAsCSV(256, "Grayscale");
			bt->stop(); bt->store("SaveCSV_bin256_gray");

			std::cout<< "Saving grayscale image-histogram(256) succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
			bt->reset(); bt->start();
			grayScaleImage.EqualizeImage(256);
			bt->stop(); bt->store("Equalize_image_bin256");

			std::cout << "Equalized image. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

			ss.str("");
			ss << "equalized_" <<grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as png

			bt->reset(); bt->start();
			save = grayScaleImage.saveToFile(ss.str());
			bt->stop(); bt->store("Save_Equalized");
			if(save)
			{
				std::cout<< "Saving image succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
			}
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
	invertedImage.convertToColor(Image::INVERTED);
	bt->stop();	bt->store("Convert_Inverted");

	std::cout << "Inverting image colors. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

	ss.str("");
	ss << "inverted_" <<invertedImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	bool save = invertedImage.saveToFile(ss.str());
	bt->stop();	bt->store("Save_inverted");

	if(save)
	{
		std::cout<< "Saving image succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
	}
	else 
	{
		stop("Saving image failed.");
	}
}

void colorRoutine()
{
	ss.str("");
#pragma region Red
	std::cout << "Converting the image to R(ed) color channel." << std::endl;

	bt->reset(); bt->start();
	Image redImage = Image(originalImage);
	bt->stop();	bt->store("Copy_originalImage");

	bt->reset(); bt->start();
	redImage.convertToColor(Image::RED);
	bt->stop();	bt->store("Convert_Red");

	ss << "R_" << redImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	bool save = redImage.saveToFile(ss.str());
	bt->stop();	bt->store("Save_Red");

	if(save)
	{
		std::cout<< "Saving image RED succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

		bt->reset(); bt->start();
		save = redImage.calculateBins(10, Image::RED);
		bt->stop();	bt->store("Calc_bin10_red");

		if(save)
		{
			bt->reset(); bt->start();
			redImage.saveHistogramAsCSV(10, "Red");
			bt->stop();	bt->store("SaveCSV_bin256_red");

			std::cout<< "Saving red image-histogram(10) succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
		}
	}
#pragma endregion

#pragma region Green
	std::cout << "Converting the image to G(reen) color channel." << std::endl;

	bt->reset(); bt->start();
	Image greenImage = Image(originalImage);
	bt->stop();	bt->store("Copy_originalImage");

	bt->reset(); bt->start();
	greenImage.convertToColor(Image::GREEN);
	bt->stop();	bt->store("Convert_Green");

	ss << "G_" << greenImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	save = greenImage.saveToFile(ss.str());
	bt->stop();	bt->store("Save_Green");

	if(save)
	{
		std::cout<< "Saving image Green succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

		bt->reset(); bt->start();
		save = greenImage.calculateBins(10, Image::RED);
		bt->stop();	bt->store("Calc_bin10_green");

		if(save)
		{
			bt->reset(); bt->start();
			greenImage.saveHistogramAsCSV(10, "Green");
			bt->stop();	bt->store("SaveCSV_bin256_green");

			std::cout<< "Saving green image-histogram(10) succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
		}
	}
#pragma endregion

#pragma region Blue
	std::cout << "Converting the image to B(lue) color channel." << std::endl;
	bt->reset(); bt->start();
	Image blueImage = Image(originalImage);
	bt->stop();	bt->store("Copy_originalImage");

	bt->reset(); bt->start();
	blueImage.convertToColor(Image::GREEN);
	bt->stop();	bt->store("Convert_Blue");

	ss << "B_" << blueImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	save = blueImage.saveToFile(ss.str());
	bt->stop();	bt->store("Save_Blue");

	if(save)
	{
		std::cout<< "Saving image Blue succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;

		bt->reset(); bt->start();
		save = blueImage.calculateBins(10, Image::BLUE);
		bt->stop();	bt->store("Calc_bin10_blue");

		if(save)
		{
			bt->reset(); bt->start();
			blueImage.saveHistogramAsCSV(10, "Blue");
			bt->stop();	bt->store("SaveCSV_bin256_blue");

			std::cout<< "Saving blue image-histogram(10) succeeded. (in " << bt->elapsedMilliSeconds() << " miliseconds)" << std::endl;
		}
	}
#pragma endregion
}

void stop(std::string msg)
{
	std::cout << msg << std::endl << "Press enter to stop.." << std::endl;
	std::cin.get();
	exit(0);
}