#include "TransformationMatrix.h"

/*
reads the file and constructs a matrix.
file contents have to be 3x3:

a0	a1	a2
b0	b1	b2
c0	c1	c2
*/
TransformationMatrix::TransformationMatrix(std::string filename)
{
	std::fstream matrixFile(filename, std::ios::in);
	float floatCharacter;
	while (matrixFile >> floatCharacter) {
		transformMatrix.push_back(floatCharacter);
	}
	calculateInverseMatrix(); // might as well do this right away (where not gonna use the normal matrix, only inverses)..
}

TransformationMatrix::TransformationMatrix(std::vector<float> matrixData)
{
	transformMatrix = std::vector<float>(matrixData);
	calculateInverseMatrix(); // might as well do this right away (where not gonna use the normal matrix, only inverses)..
}

TransformationMatrix::~TransformationMatrix(void)
{
}

void TransformationMatrix::addTransformationMatrix(const TransformationMatrix &rhs)
{
	std::vector<float> matrixData(9,0);
	std::vector<float> inverseMatrixData(9,0);

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			for(int k = 0; k < 3; k++)
			{
				//set both matrices.
				matrixData[ i * 3 + j ] += this->transformMatrix[i * 3 + k] * rhs.transformMatrix[k * 3 + j];
				inverseMatrixData[ i * 3 + j ] += this->inverseTransformMatrix[i * 3 + k] * rhs.inverseTransformMatrix[k * 3 + j];
			}
		}
	}
	transformMatrix = matrixData;
	inverseTransformMatrix = inverseMatrixData;
}

std::vector<float> TransformationMatrix::getTransformationMatrix()
{
	return transformMatrix;
}

std::vector<float> TransformationMatrix::getInverseTransformationMatrix()
{
	return inverseTransformMatrix;
}

void TransformationMatrix::calculateInverseMatrix()
{

	float a = transformMatrix.at(0), b = transformMatrix.at(1), c = transformMatrix.at(2), 
		d = transformMatrix.at(3), e = transformMatrix.at(4), f = transformMatrix.at(5),
		g = transformMatrix.at(6), h = transformMatrix.at(7), k = transformMatrix.at(8);

	float determinant = (a * e * k) + (b * f * g) + (c * d * h) - (c * e * g) - (a * f * h) - (b * d * k);

	if(determinant > 0)
	{
		// we need to push_back in correct order.
		//Matrix layout:
		// A - D - G
		// B - E - H
		// C - F - K
		inverseTransformMatrix.push_back((1 / determinant) * (e*k - f*h)); // A
		inverseTransformMatrix.push_back((1 / determinant) * (c*h - b*k)); // D
		inverseTransformMatrix.push_back((1 / determinant) * (b*f - c*e)); // G

		inverseTransformMatrix.push_back((1 / determinant) * (f*g - d*k)); // B
		inverseTransformMatrix.push_back((1 / determinant) * (a*k - c*g)); // E
		inverseTransformMatrix.push_back((1 / determinant) * (c*d - a*f)); // H

		inverseTransformMatrix.push_back((1 / determinant) * (d*h - e*g)); // C
		inverseTransformMatrix.push_back((1 / determinant) * (g*b - a*h)); // F
		inverseTransformMatrix.push_back((1 / determinant) * (a*e - b*d)); // K

	} 
	else 
	{
		//cant inverse this matrix. Use forward mapping?

		//Nah..

		//throw error
		throw Exception("Cant inverse this matrix.", __LINE__);
	}
}