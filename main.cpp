#include <iostream>

#include "TestTimer.h"
#include "imagemath.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Enter file path!" << std::endl;
		return 1;
	}
	std::cout.precision(5);

	char* inputFile = argv[1];
	char* outputFile = "out.ELI";
	std::cout << "inputFile <-- " << inputFile << std::endl;

	EliImage::Image input = EliImage::create(argv[1]);
	EliImage::Image output(input);
	double sigma = Math::FindSigma(input);
	std::cout << "sigma " << sigma << std::endl;

	Math::Filter kernel = Math::getGaussianFilter(3, 3, 1, sigma);
	/// filter ///
	std::cout << std::endl;
	std::cout << "---------------------------" << std::endl;
	for (int i = 0; i < kernel.size.h; ++i)
	{
		for (int j = 0; j < kernel.size.w; ++j)
		{
			std::cout << "|" << kernel.mat[i][j] << "|";
		}
		std::cout << std::endl;
		std::cout << "---------------------------" << std::endl;
	}
	/// filter ///
	{
		Timer t;
		Math::applyFilter(input, output, kernel);
	}
	{
		Timer t;
		Math::applyFilterAsync(input, output, kernel);
	}

	std::cout << "outputFile --> " << outputFile << std::endl;
	return 0;
}
