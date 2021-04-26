#ifndef ELIIMAGEPROCESSING_H
#define ELIIMAGEPROCESSING_H
#include "eliimage.h"
#include "eliimagehandler.h"
#include "string"

template<typename T>
struct argthreadData;


class EliImageProcessing
{
public:
	EliImageProcessing(const EliImageHandler &handler) { m_handler = handler; };
	bool divisionPxels(const std::string &file1, const std::string &file2, const std::string &output);
	bool divisionPxelsMultiThread(const std::string &file1, const std::string &file2, const std::string &output);
	int getNumberOfThreads(const int imageWidth, const int imageHeight);
private:
	template<typename T>
	void divisionThreadFunc(struct argthreadData<T> *array);
	EliImageHandler m_handler;
};

#endif // ELIIMAGEPROCESSING_H
