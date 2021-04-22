#ifndef ELIIMAGEPROCESSING_H
#define ELIIMAGEPROCESSING_H
#include "eliimage.h"
#include "eliimagehandler.h"
#include <QString>



class EliImageProcessing
{
public:
	EliImageProcessing() {};
	static bool divisionPxels(const QString &file1, const QString file2, const QString &output);
	static bool divisionPxelsMultiThread(const QString &file1, const QString file2, const QString &output);
	static int getNumberOfThreads(const int imageWidth, const int imageHeight);
private:
};

#endif // ELIIMAGEPROCESSING_H
