#include "AffineTransform.h"


AffineTransform::AffineTransform(ColorEnum color, Image *image, TransformationMatrix* matrix) : 
	imagePtr(image),
	matrix(matrix)
{
	imageWidth = originalImageWidth = imagePtr->getWidth();
	imageHeight = originalImageHeight = imagePtr->getHeight();
	imageData = imagePtr->getImagePixelData(); //pointer to our data. ( we need pixels. ( RGB ) )
}


AffineTransform::~AffineTransform(void)
{
}

void AffineTransform::setInterpolationMethod(interpolationEnum interpolation)
{
	interpolationType = interpolation;
}

Image AffineTransform::doTransformation()
{
	BoundingBox box = getBoundingBox(matrix->getTransformationMatrix()); // first calculate a bounding box for the image (apply transform(forward mapped, not inverse) to the 4 squares).
	imageWidth = box.getWidth();
	imageHeight = box.getHeight();

	static const float arr[] = { 1, 0, -box.minX, 0, 1, box.minY, 0, 0, 1}; // translate the bounding box 
	std::vector<float> translationMatrixData (arr, arr + sizeof(arr) / sizeof(arr[0]) );
	TransformationMatrix translationMatrix = TransformationMatrix(translationMatrixData); //create new matrix to use in translation

	matrix->addTransformationMatrix(translationMatrix); // multiply matrices

	std::vector<float> inverseTransformationMatrix = matrix->getInverseTransformationMatrix(); // take matrix for use in backward mapping

	Image resultImage = Image(imageWidth, imageHeight, imagePtr->getFileName());		
	Pixel* colorData = new Pixel[imageWidth * imageHeight];

	//std::vector<Pixel> colorData = std::vector<Pixel>(imageWidth * imageHeight, Pixel(0,0,0,0)); // initialize empty image.
	//foreach pixel in image
	for (int x = 0; x < imageWidth; x++) {
		for (int y = 0; y < imageHeight; y++) {

			//maps the source backwards(thru inverse matrix)
			float sourceX = (inverseTransformationMatrix[0] * x) + (inverseTransformationMatrix[1] * y) + inverseTransformationMatrix[2];
			float sourceY = (inverseTransformationMatrix[3] * x) + (inverseTransformationMatrix[4] * y) + inverseTransformationMatrix[5];

			//std::cout << "Translated the new (x, y) : (" << x << ", " << y << ") to source (x, y) : (" << sourceX << ", " << sourceY << ")" << std::endl;

			if(!(sourceX < 0 || 
				sourceY < 0 || 
				sourceX >= originalImageWidth - 1 || 
				sourceY >= originalImageHeight - 1)) {
					colorData[ (y * imageWidth) + x ] = interpolate(sourceX, sourceY);
			}
		}
	}
	resultImage.setImagePixelData(colorData);
	return resultImage;
}

//Microsoft doesnt provide the C++11 defined math functions. ffs.
inline int AffineTransform::round(float number)
{
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

inline Pixel AffineTransform::interpolate(float sourceX, float sourceY)
{
	if(interpolationType == ZERO_ORDER)
	{
		//Take the closest pixel value.
		const Pixel sourcePixel = imageData[ (round(sourceY) * originalImageWidth) + round(sourceX) ]; 
		return Pixel(sourcePixel.r, sourcePixel.g, sourcePixel.b, 0); 
	} 
	else if(interpolationType == FIRST_ORDER)
	{
		//need to construct a new pixel from the 4 surrounding pixels.
		int px = (int)sourceX; // floor of x
		int py = (int)sourceY; // floor of y

		// load the neighboring pixels
		const Pixel pixel1 = imageData[ px + py * originalImageWidth ];
		const Pixel pixel2 = imageData[ (px + 1) + py * originalImageWidth];
		const Pixel pixel3 = imageData[ px + (py + 1) * originalImageWidth];
		const Pixel pixel4 = imageData[ (px + 1) + (py + 1) * originalImageWidth];

		// Calculate weights
		float fx = sourceX - px;
		float fy = sourceY - py;
		float fx1 = 1.0f - fx;
		float fy1 = 1.0f - fy;

		int w1 = fx1 * fy1 * 256.0f;
		int w2 = fx  * fy1 * 256.0f;
		int w3 = fx1 * fy  * 256.0f;
		int w4 = fx  * fy  * 256.0f;

		// Calculate the sum
		int outr = pixel1.r * w1 + pixel2.r * w2 + pixel3.r * w3 + pixel4.r * w4;
		int outg = pixel1.g * w1 + pixel2.g * w2 + pixel3.g * w3 + pixel4.g * w4;
		int outb = pixel1.b * w1 + pixel2.b * w2 + pixel3.b * w3 + pixel4.b * w4;

		return Pixel(outr >> 8, outg >> 8, outb >> 8, 0);
	}
	else 
	{
		//Throw Exception
	}
}

BoundingBox AffineTransform::getBoundingBox(const std::vector<float> matrix)
{
	std::vector<float> x(4, 0);
	std::vector<float> y(4, 0);

	//Take the 4 corners and apply the transform.
	x[0] = (matrix[0] * 0) + (matrix[1] * 0) + matrix[2]; // <0,0>
	y[0] = (matrix[3] * 0) + (matrix[4] * 0) + matrix[5];

	x[1] =  (matrix[0] * imageWidth) + (matrix[1] * 0) + matrix[2]; // <imwidth, 0>
	y[1] = (matrix[3] * imageWidth) + (matrix[4] * 0) + matrix[5];

	x[2] =  (matrix[0] * 0) + (matrix[1] * imageHeight) + matrix[2];// <0, imheight>
	y[2] = (matrix[3] * 0) + (matrix[4] * imageHeight) + matrix[5];

	x[3] = (matrix[0] * imageWidth) + (matrix[1] * imageHeight) + matrix[2];// <imwidth, imheight>
	y[3] = (matrix[3] * imageWidth) + (matrix[4] * imageHeight) + matrix[5];

	int minX = *std::min_element(x.begin(), x.end()); //from float -> int. I expect truncation (same as using floor(x))
	int maxX = *std::max_element(x.begin(), x.end());
	int minY = *std::min_element(y.begin(), y.end());
	int maxY = *std::max_element(y.begin(), y.end());

	//bounding box is (min_x, min_y), (min_x, max_y), (max_x, max_y), (max_x, min_y)
	return BoundingBox(minX, maxX, minY, maxY);
}