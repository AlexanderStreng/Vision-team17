#include <iostream>
#include "Image.h"
#include "TransformationMatrix.h"
#include "exectimer.h"
#include "basetimer.h"
#include "Globals.h"
#include "AffineTransform.h"
#include <random>
#include <time.h>       /* time */

std::string filename, matrixFile;
int interpolationType;
interpolationEnum interpolationTypeEnum;
Image originalImage;
std::stringstream ss;
BaseTimer* bt;

bool exerciseRoutine();
void affineTransformRoutine();
void stop(std::string msg);
void MeasurementsRoutine1();
void MeasurementsRoutine2();

int main(int argc, char* argv[])
{
	//check argv[1] for input, if not, ask for input.
	if(argc < 2)
	{
		std::cout << std::endl << "No filename found. Could you please insert the (path+)filename? " << std::endl;
		getline(std::cin, filename);
		std::cout << std::endl << "No matrix file found. Could you please insert the (path+)filename? " << std::endl;
		getline(std::cin, matrixFile);
		std::cout << std::endl << "No interpolationmethod found. Could you please insert interpolation method? (0) for zero-order(default) and (1) for first order." << std::endl;
		char option = getchar();
		if(option == '0')
		{
			interpolationType = 0;
		}
		else if(option == '1')
		{
			interpolationType = 1;
		}
		else 
		{
			std::cout << std::endl << "Input could not be read. Press enter to exit." << std::endl;
			std::cin.get();
			exit(0);
		}
	} 
	else
	{		
		std::cout << "Command line arguments found." << std::endl;
		filename = argv[1];
		matrixFile = argv[2];
		interpolationType = atoi(argv[3]);
	}
	interpolationTypeEnum = interpolationType != 1 ? ZERO_ORDER : FIRST_ORDER;
	while(getchar()!='\n'); // clear input buffer to prevent from a screen showing twice.
	std::cout << "I am going to use the file: '" << filename << "' with the matrix: " << matrixFile << " and interpolationType: " << interpolationType << "-order" << std::endl;

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
		std::cout << "Press (1) to run this application. Images are backward mapped and coordinate space is enlarged." << std::endl
			<< "Press (0) to quit this application." << std::endl << std::endl;

		char newMeans;
		char option = getchar();
		switch(option) {
		case '1':
			affineTransformRoutine();
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

void affineTransformRoutine(){
	//prepare image
	bt->reset(); bt->start();
	Image sourceImage = Image(originalImage);
	bt->stop(); bt->store("Copy_originalImage");

	bt->reset(); bt->start();	
	TransformationMatrix transformMatrix = TransformationMatrix(matrixFile);
	bt->stop(); bt->store("Read_transformMatrix");

	bt->reset(); bt->start();	
	//do transformation
	AffineTransform transform = AffineTransform(FULLCOLOUR, &sourceImage, &transformMatrix, interpolationTypeEnum);
	Image transformedImage = transform.doTransformation();
	bt->stop(); bt->store("Do_Transform");

	std::cout << "Done transformation. \t(in " << bt->elapsedMilliSeconds() << " milliseconds)" << std::endl;

	ss.str("");
	ss << "transformed_" << transformedImage.getFileNameWithoutExtension() << ".png"; // save as png

	bt->reset(); bt->start();
	transformedImage.saveToFile(ss.str(), FULLCOLOUR);
	bt->stop();	bt->store("Save_transformed");
}

//Meetrapport routines
void MeasurementsRoutine1() //filter volgorde
{
	bt = new BaseTimer();
	Image originalImage = Image("cat-transforms.jpg");
	{
		TransformationMatrix rotateMatrix = TransformationMatrix("rotate-meetrapport.txt");
		AffineTransform rotateZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &rotateMatrix, ZERO_ORDER);
		Image transformedrotateZeroOrderImage = rotateZeroOrder.doTransformation();
		ss.str(""); ss << "rotate - 0.png"; // save as png
		transformedrotateZeroOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		rotateMatrix = TransformationMatrix("rotate-meetrapport.txt");
		AffineTransform rotateFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &rotateMatrix, FIRST_ORDER);
		Image transformedrotateFirstOrderImage = rotateFirstOrder.doTransformation();
		ss.str(""); ss << "rotate - 1 - diff - " << transformedrotateZeroOrderImage.compareToImage(&transformedrotateFirstOrderImage, FULLCOLOUR) << "%.png"; // save as png
		transformedrotateFirstOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		std::cout << "Done Rotate" << std::endl;
	}

	{
		TransformationMatrix scaleMatrix = TransformationMatrix("scale-meetrapport.txt");
		AffineTransform scaleZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &scaleMatrix, ZERO_ORDER);
		Image transformedscaleZeroOrderImage = scaleZeroOrder.doTransformation();
		ss.str(""); ss << "scale - 0.png"; // save as png
		transformedscaleZeroOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		scaleMatrix = TransformationMatrix("scale-meetrapport.txt");
		AffineTransform scaleFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &scaleMatrix, FIRST_ORDER);
		Image transformedscaleFirstOrderImage = scaleFirstOrder.doTransformation();
		ss.str(""); ss << "scale - 1 - diff - " << transformedscaleZeroOrderImage.compareToImage(&transformedscaleFirstOrderImage, FULLCOLOUR) << "%.png"; // save as png
		transformedscaleFirstOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		std::cout << "Done scale" << std::endl;
	}

	{
		TransformationMatrix shearMatrix = TransformationMatrix("shear-meetrapport.txt");
		AffineTransform shearZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &shearMatrix, ZERO_ORDER);
		Image transformedshearZeroOrderImage = shearZeroOrder.doTransformation();
		ss.str(""); ss << "shear - 0.png"; // save as png
		transformedshearZeroOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		shearMatrix = TransformationMatrix("shear-meetrapport.txt");
		AffineTransform shearFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &shearMatrix, FIRST_ORDER);
		Image transformedshearFirstOrderImage = shearFirstOrder.doTransformation();
		ss.str(""); ss << "shear - 1 - diff - " << transformedshearZeroOrderImage.compareToImage(&transformedshearFirstOrderImage, FULLCOLOUR) << "%.png"; // save as png
		transformedshearFirstOrderImage.saveToFile(ss.str(), FULLCOLOUR);

		std::cout << "Done shear" << std::endl;
	}

}
//Meetrapport routines
void MeasurementsRoutine2() //histogram equalisation
{
	bt = new BaseTimer();
	Image originalImage = Image("cat-transforms.jpg");

	for(int i = 0; i < 100; i++)
	{
		{
			TransformationMatrix rotateMatrix = TransformationMatrix("rotate-meetrapport.txt");
			AffineTransform rotateZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &rotateMatrix, ZERO_ORDER);
			ss.str(""); ss << "rotate0 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedrotateZeroOrderImage = rotateZeroOrder.doTransformation();
			bt->stop();	bt->store(ss.str());

			rotateMatrix = TransformationMatrix("rotate-meetrapport.txt");
			AffineTransform rotateFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &rotateMatrix, FIRST_ORDER);
			ss.str(""); ss << "rotate1 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedrotateFirstOrderImage = rotateFirstOrder.doTransformation();
			bt->stop();	bt->store(ss.str());
		}

		{
			TransformationMatrix scaleMatrix = TransformationMatrix("scale-meetrapport.txt");
			AffineTransform scaleZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &scaleMatrix, ZERO_ORDER);
			ss.str(""); ss << "scale0 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedscaleZeroOrderImage = scaleZeroOrder.doTransformation();
			bt->stop();	bt->store(ss.str());

			scaleMatrix = TransformationMatrix("scale-meetrapport.txt");
			AffineTransform scaleFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &scaleMatrix, FIRST_ORDER);
			ss.str(""); ss << "scale1 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedscaleFirstOrderImage = scaleFirstOrder.doTransformation();
			bt->stop();	bt->store(ss.str());
		}

		{
			TransformationMatrix shearMatrix = TransformationMatrix("shear-meetrapport.txt");
			AffineTransform shearZeroOrder = AffineTransform(FULLCOLOUR, &originalImage, &shearMatrix, ZERO_ORDER);
			ss.str(""); ss << "shear0 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedshearZeroOrderImage = shearZeroOrder.doTransformation();
			bt->stop();	bt->store(ss.str());

			shearMatrix = TransformationMatrix("shear-meetrapport.txt");
			AffineTransform shearFirstOrder = AffineTransform(FULLCOLOUR, &originalImage, &shearMatrix, FIRST_ORDER);
			ss.str(""); ss << "shear1 - iteration-" << ((i < 10) ? "0" : "") << i;
			bt->reset(); bt->start();	
			Image transformedshearFirstOrderImage = shearFirstOrder.doTransformation();
			bt->stop();	bt->store(ss.str());
		}
		std::cout << "Done " << i << " iterations." << std::endl;
	}
	ss.str("");
	ss << "afinneperformance.csv"; // save as png
	bt->save(ss.str());
}