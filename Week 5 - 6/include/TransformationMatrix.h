#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Globals.h"

class TransformationMatrix
{
public:
	TransformationMatrix();
	TransformationMatrix(std::string filename);
	TransformationMatrix(std::vector<float> matrixData);
	void addTransformationMatrix(const TransformationMatrix &rhs);
	~TransformationMatrix(void);
	std::vector<float> getTransformationMatrix();
	std::vector<float> getInverseTransformationMatrix();
private:
	void calculateInverseMatrix();
	std::vector<float> transformMatrix; //matrix contents a0, a1, a2, b0, b1, b2, c0, c1, c2;
	std::vector<float> inverseTransformMatrix; //matrix contents a0, a1, a2, b0, b1, b2, c0, c1, c2;
};

