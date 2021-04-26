#ifndef ELIIMAGEPROCESSING_H
#define ELIIMAGEPROCESSING_H
#include "eliimage.h"
#include "eliimagehandler.h"
#include <QString>

template<typename T>
struct argthreadData;


class EliImageProcessing
{
public:
	EliImageProcessing(const EliImageHandler &handler) { m_handler = handler; };
	bool divisionPxels(const QString &file1, const QString file2, const QString &output);
	bool divisionPxelsMultiThread(const QString &file1, const QString file2, const QString &output);
	int getNumberOfThreads(const int imageWidth, const int imageHeight);
private:
	template<typename T>
	void divisionThreadFunc(struct argthreadData<T> *array);
	EliImageHandler m_handler;
};

#endif // ELIIMAGEPROCESSING_H
