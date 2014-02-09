#include <iostream>
#include "Image.h"

std::string filename, yesOrNo;
Image originalImage;

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
	originalImage = Image(filename);

	if (!originalImage.Excists()) {
		stop("Image could not be loaded");
	} 

	std::cout << "Loaded img:" << originalImage.getFileNameWithoutExtension() << " dimensions(WxH):"  << originalImage.getWidth() << " x " << originalImage.getHeight() << "." << std::endl;

	grayScaleRoutine();
	colorRoutine();
	invertRoutine();

	stop("");
}

void grayScaleRoutine()
{
	std::cout << "Converting the image to grayscale." << std::endl;
	Image grayScaleImage = Image(filename);
	grayScaleImage.convertToColor(Image::GRAYSCALE);
	std::stringstream ss;
	ss << "grey_" <<grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as png

	if(grayScaleImage.saveToFile(ss.str()))
	{
		std::cout<< "Saving image succeeded." << std::endl;
		// create 2 bins (10 - 256)
	}
	else 
	{
		stop("Saving image failed.");
	}
}

void invertRoutine()
{
	std::cout << "Inverting image colors." << std::endl;
	Image invertedImage = Image(filename);
	invertedImage.convertToColor(Image::INVERTED);
	std::stringstream ss;
	ss << "inverted_" <<invertedImage.getFileNameWithoutExtension() << ".png"; // save as png

	if(invertedImage.saveToFile(ss.str()))
	{
		std::cout<< "Saving image succeeded." << std::endl;
	}
	else 
	{
		stop("Saving image failed.");
	}
}

void colorRoutine()
{
	std::stringstream ss;

	std::cout << "Converting the image to R(ed) color channel." << std::endl;
	Image redImage = Image(filename);
	redImage.convertToColor(Image::RED);
	ss << "R_" << redImage.getFileNameWithoutExtension() << ".png"; // save as png
	if(redImage.saveToFile(ss.str()))
	{
		std::cout<< "Saving RedChannelImage image succeeded." << std::endl;
	}

	std::cout << "Converting the image to G(reen) color channel." << std::endl;
	Image greenImage = Image(filename);
	greenImage.convertToColor(Image::GREEN);
	ss.str("");
	ss << "G_" << greenImage.getFileNameWithoutExtension() << ".png"; // save as png
	if(greenImage.saveToFile(ss.str()))
	{
		std::cout<< "Saving image succeeded." << std::endl;
	}

	std::cout << "Converting the image to B(lue) color channel." << std::endl;
	Image blueImage = Image(filename);
	blueImage.convertToColor(Image::BLUE);
	ss.str("");
	ss << "B_" << blueImage.getFileNameWithoutExtension() << ".png"; // save as png
	if(blueImage.saveToFile(ss.str()))
	{
		std::cout<< "Saving image succeeded." << std::endl;
	}
}

void stop(std::string msg)
{
	std::cout << msg << std::endl << "Press enter to stop.." << std::endl;
	std::cin.get();
	exit(0);
}