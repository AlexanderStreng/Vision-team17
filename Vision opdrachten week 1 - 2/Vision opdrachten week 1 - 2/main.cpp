#include <iostream>
#include "Image.h"

void stop(std::string msg); //forward declare this function.

int main(int argc, char* argv[])
{
	std::string filename, yesOrNo, grayOrColor;
	Image originalImage;

	//check argv[1] for input, if not, ask for input.
	if(argc < 2)
	{
		std::cout << "No command line arguments found. Could you please insert the filename now? " << std::endl;
		getline(std::cin, filename);
	} 
	else
	{		
		std::cout << "Command line argument found." << std::endl;
		filename = argv[1];
	}

	std::cout << "I am going to use the file: '" << filename << "' is this correct?(y/n)" << std::endl;
	getline(std::cin, yesOrNo);

	if(yesOrNo != "y" && yesOrNo != "yes") //Trying to keep this simple. If no other input then y(es) is given, lets just quit.
	{
		stop("Not using this file..");
	}

	originalImage = Image(filename); //since we are allowed to only use a lib for loading images, lets open in our own class, with which we can have some fun.

	if (!originalImage.Excists()) {
		stop("Image could not be loaded");
	} 
	else
	{
		std::cout << "Loaded img:" << originalImage.getFileNameWithoutExtension() << " dimensions(wxh):"  << originalImage.getWidth() << " x " << originalImage.getHeight() << "." << std::endl;
	}

	std::cout << "Do you want to convert the image to grayscale(g) or seperate color channels?(c) (g/c)" << std::endl;
	getline(std::cin, grayOrColor);

	if(grayOrColor == "g") 
	{
		std::cout << "Converting the image to grayscale." << std::endl;
		Image grayScaleImage = Image(originalImage);
		grayScaleImage.convertToGrayScale();

		std::stringstream ss;
		ss << "grey_" <<grayScaleImage.getFileNameWithoutExtension() << ".png"; // save as bmp
		if(grayScaleImage.saveToFile(ss.str(), Image::GREYSCALE))
		{
			stop("Saving image succeeded.");
		}
		else 
		{
			stop("Saving image failed.");
		}
	}
	else if(grayOrColor == "c")
	{
		std::cout << "Converting the image to R(ed) G(reen) and B(lue) color channels." << std::endl;
	}
	else
	{
		stop("No suitable input found. Please use 'g' for grayscale, and 'c' for color channels next time.");
	}
}

void stop(std::string msg)
{
	std::cout << msg << std::endl << "Press any key to stop.." << std::endl;
	std::cin.get();
	exit(0);
}