#include "Image.h" //Still might want to prevent circular dependencies
#include <iostream> 
#include <fstream>
#include <Math.h>

class Histogram
{
	private:
		int bins;
		int* dataGray;
	public:
		Histogram(int bins);
		bool saveAsCSV(std::string filename, Image::OutputColorEnum color);
		bool calculateGrayBins(Image image);
		~Histogram();
};

