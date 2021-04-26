#include "eliimageprocessing.h"
#include "iostream"
#include "thread"
#include "type_traits"
#include "future"
#include "functional"

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
void EliImageProcessing::divisionThreadFunc(argthreadData<T> *array)
{
	for (int y = array->startPos; y < array->startPos + array->blockSize; y++)
	{
		array->resultData[y] = new T[array->width];
		for (int x = 0; x < array->width; x++)
		{
			if (array->secondData[y][x] == 0)
				array->resultData[y][x] = std::numeric_limits<T>::max();
			else
				array->resultData[y][x] = array->firstData[y][x] / array->secondData[y][x];
		}
	}
}


bool EliImageProcessing::divisionPxels(const std::string &file1, const std::string &file2, const std::string &output)
{
	EliImageHeader header1, header2;
	if (!(m_handler.readHeader(file1,header1) && m_handler.readHeader(file2,header2)))
	{
		return false;
	}
	if (header1.bit_count != header2.bit_count)
	{
		std::cout << "Разные форматы изображений";
		return false;
	}
	if (header1.size() != header2.size())
	{
		std::cout << "Разные размеры изображений";
		return false;
	}

	EliImage<uint16_t> img1(header1), img2(header2);
	m_handler.read16bitEli(file1, img1);
	m_handler.read16bitEli(file2, img2);
	EliImage<uint16_t> resultImg(header1);
	auto width = img1.header().image_width;
	auto height= img1.header().image_height;
	uint16_t **resultData = new uint16_t*[width];
	for (int y = 0; y < width; y++)
	{
		resultData[y] = new uint16_t[height];
		for (int x = 0; x < height; x++)
		{
			if (img2.data()[y][x] == 0)
				resultData[y][x] = std::numeric_limits<uint16_t>::max();
			else
			resultData[y][x] = img1.data()[y][x] / img2.data()[y][x];
		}
	}
	resultImg.setData(resultData);
	return m_handler.write16bitEli(resultImg, output);
}


bool EliImageProcessing::divisionPxelsMultiThread(const std::string &file1, const std::string &file2, const std::string &output)
{
	EliImageHeader header1, header2;
	if (!(m_handler.readHeader(file1,header1) && m_handler.readHeader(file2,header2)))
	{
		return false;
	}
	if (header1.bit_count != header2.bit_count)
	{
		std::cout << "Разные форматы изображений";
		return false;
	}
	if (header1.size() != header2.size())
	{
		std::cout << "Разные размеры изображений";
		return false;
	}

	EliImage<uint16_t> img1(header1), img2(header2);
	m_handler.read16bitEli(file1, img1);
	m_handler.read16bitEli(file2, img2);
	EliImage<uint16_t> resultImg(header1);
	auto width = img1.header().image_width;
	auto block = width / threadsCount;
	image_proc_thread_t th[threadsCount];
	argthreadData<uint16_t> ag[threadsCount];
	uint16_t **resultData = new uint16_t*[width];
	auto threadFunction = std::bind(&EliImageProcessing::divisionThreadFunc<uint16_t>, this, std::placeholders::_1);
	for (unsigned i = 0; i < threadsCount; i++)
	{
		ag[i].width = width;
		ag[i].startPos = (i * block);
		ag[i].blockSize = block;
		ag[i].firstData = img1.data();
		ag[i].secondData = img2.data();
		ag[i].resultData = resultData;
	}
	auto remnant = width % threadsCount;
	auto lastThread = threadsCount - 1;
	ag[lastThread].blockSize = block + remnant;

	for (unsigned i = 0; i < threadsCount; i++)
	{
		image_proc_thread_create<uint16_t, argthreadData>(&th[i], threadFunction, &ag[i]);
		image_proc_thread_wait(th[i]);
	}
	resultImg.setData(resultData);
	return m_handler.write16bitEli(resultImg, output);
}

