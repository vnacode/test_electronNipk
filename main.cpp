#include <QCoreApplication>
#include "iostream"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	if (argc < 2)
	{
		std::cout << "Введите имя файлов изображений!!" << std::endl;;
	}

	return 0;
}
