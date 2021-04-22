#include "iostream"
#include <QCoreApplication>
#include "eliimageprocessing.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if (argc < 2)
	{
		std::cout << "Введите имя файлов изображений!!" << std::endl;;
	}
	EliImageProcessing::divisionPxels(argv[1], argv[2], argv[3]);
//	handler.readHeader(argv[1], header);
	return 0;
}
