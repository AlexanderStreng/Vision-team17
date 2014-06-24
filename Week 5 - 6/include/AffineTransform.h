#pragma once

#include "globals.h"
#include "Image.h"
#include "TransformationMatrix.h"
#include <algorithm>

class AffineTransform
{
public:
	AffineTransform(ColorEnum color, Image *image, TransformationMatrix *matrix);
	~AffineTransform(void);
	Image doTransformation();
	void setInterpolationMethod(interpolationEnum interpolation);
private:
	inline Pixel interpolate(float sourceX, float sourceY);
	interpolationEnum interpolationType;
	int imageWidth, originalImageWidth;
	int imageHeight, originalImageHeight;
	Image* imagePtr;
	Pixel* imageData;
	TransformationMatrix* matrix;
	inline int round(float number);
	BoundingBox getBoundingBox(const std::vector<float> transformationMatrixData);
};

