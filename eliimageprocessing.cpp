#include "eliimageprocessing.h"
#include "thread"
#include <QDebug>
#include "type_traits"

unsigned int threadsCount = std::thread::hardware_concurrency();


using image_proc_thread_t = std::future<void>;

template <typename T>
using pthreadFunction = std::function<void(T*)>;


template<class T, template<class> class argst>
void image_proc_thread_create(image_proc_thread_t* thp, pthreadFunction<argst<T>> func, argst<T>* argp)
{
	*thp = std::async(func, argp);
}


void image_proc_thread_wait(image_proc_thread_t& th)
{
	th.wait();
}

template <typename T>
struct argthreadData
{
	T **firstData;
	T **secondData;
	T **resultData;
	int blockSize;
	int width;
	int startPos;
};

template <typename T>
void divisionThreadFunc(argthreadData<quint16> *array)
{
	for (int y = array->startPos; y < array->startPos + array->blockSize; y++)
	{
//		delete array->resultData[y];
		array->resultData[y] = new T[array->width];
		for (int x = 0; x < array->width; x++)
		{
			array->resultData[y][x] = array->firstData[y][x] / array->secondData[y][x];
		}
	}
}


bool EliImageProcessing::divisionPxels(const QString &file1, const QString file2, const QString &output)
{
	EliImageHeader header1, header2;
	if (!(m_handler.readHeader(file1,header1) && m_handler.readHeader(file2,header2)))
	{
		return false;
	}
	if (header1.bit_count != header2.bit_count)
	{
		qWarning() << "Разные форматы изображений";
		return false;
	}
	if (header1.size() != header2.size())
	{
		qWarning() << "Разные размеры изображений";
		return false;
	}

	EliImage<quint16> img1(header1), img2(header2);
	m_handler.read16bitEli(file1, img1);
	m_handler.read16bitEli(file2, img2);
	EliImage<quint16> resultImg(header1);
	auto width = img1.header().image_width;
	auto height= img1.header().image_height;
	quint16 **resultData = new quint16*[width];
	for (int y = 0; y < width; y++)
	{
		resultData[y] = new quint16[height];
		for (int x = 0; x < height; x++)
		{
			//qDebug() << img1.data()[y][x] << img2.data()[y][x];
			resultData[y][x] = img1.data()[y][x] / img2.data()[y][x];
		}
	}
	resultImg.setData(resultData);
	return m_handler.write16bitEli(resultImg, output);
}


bool EliImageProcessing::divisionPxelsMultiThread(const QString &file1, const QString file2, const QString &output)
{
	EliImageHeader header1, header2;
	if (!(m_handler.readHeader(file1,header1) && m_handler.readHeader(file2,header2)))
	{
		return false;
	}
	if (header1.bit_count != header2.bit_count)
	{
		qWarning() << "Разные форматы изображений";
		return false;
	}
	if (header1.size() != header2.size())
	{
		qWarning() << "Разные размеры изображений";
		return false;
	}

	EliImage<quint16> img1(header1), img2(header2);
	m_handler.read16bitEli(file1, img1);
	m_handler.read16bitEli(file2, img2);
	EliImage<quint16> resultImg(header1);
	auto width = img1.header().image_width;
	auto block = width / threadsCount;
	image_proc_thread_t th[threadsCount];
	argthreadData<quint16> ag[threadsCount];
	quint16 **resultData = new quint16*[width];

	for (unsigned i = 0; i < threadsCount - 1; i++)
	{
		ag[i].width = width;
		ag[i].startPos = (i * block);
		ag[i].blockSize = block;
		ag[i].firstData = img1.data();
		ag[i].secondData = img2.data();
		ag[i].resultData = resultData;
		image_proc_thread_create<quint16, argthreadData>(&th[i], divisionThreadFunc<quint16>, &ag[i]);
	}
	auto remnant = width % threadsCount;
	auto lastThread = threadsCount - 1;
	ag[lastThread].width = width;
	ag[lastThread].startPos = (lastThread * block);
	ag[lastThread].blockSize = block + remnant;
	ag[lastThread].firstData = img1.data();
	ag[lastThread].secondData = img2.data();
	ag[lastThread].resultData = resultData;
	image_proc_thread_create<quint16, argthreadData>(&th[lastThread], divisionThreadFunc<quint16>, &ag[lastThread]);


	for (unsigned i = 0; i < threadsCount; i++)
	{
		image_proc_thread_wait(th[i]);
	}
	resultImg.setData(resultData);
	return m_handler.write16bitEli(resultImg, output);
}

