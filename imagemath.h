#pragma once
#include <math.h>
#include <inttypes.h>
#include <functional>
#include <numeric>
#include <future>
#include <vector>

#include "image.h"

namespace Math
{
	struct Size
	{
		Size(int w = 0, int h = 0) : w(w), h(h) {}
		Size(const Size& other)	: w(other.w), h(other.h) {}
		int w, h;
	};

	struct Rect
	{
		Rect() = default;
		Rect(Size point, Size size) : point(point), size(size) {}
		Rect(const Rect& other) : point(other.point), size(other.size) {}
		Size point;
		Size size;
	};

	struct Filter
	{
		Size size;
		double** mat = nullptr;
	};

	struct argthreadData
	{
		argthreadData(EliImage::Image* src, EliImage::Image* out, Rect rect)
			: src(src), out(out), outrect(rect) {}
		EliImage::Image* src;
		EliImage::Image* out;
		Rect outrect;
	};

	using image_proc_thread_t = std::future<void>;
	using pthreadFunction = std::function<void(argthreadData*)>;

	void image_proc_thread_create(image_proc_thread_t* thp, pthreadFunction func, argthreadData* argp)
	{
		*thp = std::async(func, argp);
	}

	void image_proc_thread_wait(image_proc_thread_t& th)
	{
		th.wait();
	}

	void applyFilter(EliImage::Image& src, EliImage::Image& dst, const Filter& filter)
	{
		if (src.Width() != dst.Width() || src.Height() != dst.Height())
		{
			std::cout << "Images is not same size!" << std::endl;
			return;
		}

		for (int y = 0; y < dst.Height(); ++y)
		{
			for (int x = 0; x < dst.Width(); ++x)
			{
				uint16_t sum = 0;
				for (int i = 0; i < filter.size.h; ++i)
				{
					for (int j = 0; j < filter.size.w; ++j)
					{
						uint16_t v = (uint16_t)(filter.mat[i][j] * (double)src.getPixel(x + j - 1, y + i - 1));
						sum += v;
					}
				}
				dst.setPixel(x, y, sum);
			}
		}
	}

	void applyFilterThread(argthreadData* argt, const Filter& filter)
	{
		auto start = argt->outrect.point;
		auto end = argt->outrect.size;
		for (int y = start.h; y < end.h; ++y)
		{
			for (int x = start.w; x < end.w; ++x)
			{
				uint16_t sum = 0;
				for (int i = 0; i < filter.size.h; ++i)
				{
					for (int j = 0; j < filter.size.w; ++j)
					{
						uint16_t v = (uint16_t)(filter.mat[i][j] * (double)argt->src->getPixel(x + j - 1, y + i - 1));
						sum += v;
					}
				}
				argt->out->setPixel(x, y, sum);
			}
		}
	}

	std::vector<Rect> divideBlock(Size size, int num)
	{
		auto width = size.w / num;
		auto lastextWith = size.w % num;
		auto height = size.h;
		std::vector<Rect> blocks;
		blocks.reserve(num);
		for (int i = 0; i < num; ++i)
		{
			blocks.push_back(Rect(Size(i * width, 0), Size(width, height)));
		}
		blocks[num-1].size.w = width + lastextWith;

		return blocks;
	}

	void applyFilterAsync(EliImage::Image& src, EliImage::Image& dst, const Filter& filter)
	{
		if (src.Width() != dst.Width() || src.Height() != dst.Height())
		{
			std::cout << "Images is not same size!" << std::endl;
			return;
		}
		const auto hardwareThreadCount = std::thread::hardware_concurrency();
		auto blocks =  divideBlock(Size(dst.Width(), dst.Height()), hardwareThreadCount);
		

		std::vector<image_proc_thread_t> workers(hardwareThreadCount);
		std::vector<argthreadData> argd;
		argd.reserve(hardwareThreadCount);
		auto threadFunction = std::bind(&applyFilterThread, std::placeholders::_1, filter);
		for (int i = 0; i < hardwareThreadCount; ++i)
		{
			argd.emplace_back(argthreadData(&src, &dst, blocks[i]));
		}
		for (int i = 0; i < hardwareThreadCount; ++i)
		{
			image_proc_thread_create(&workers[i], threadFunction, &argd[i]);
			image_proc_thread_wait(workers[i]);
		}
	}

	double FindSigma(const EliImage::Image& src)
	{
		double mean = 0;
		double sum = 0;
		size_t count = 0;
		int width = src.Width();
		int height = src.Height();

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				uint16_t pixel = src.getPixel(i, j);
				count++;
				sum += (double)pixel / (double)UINT16_MAX;
			}
		}
		mean = sum / count;
		double sigma = 0;

		for (int i = 0; i < width; ++i)
		{
			for (int j = 0; j < height; ++j)
			{
				uint16_t pixel = src.getPixel(i, j);
				double val = (double)pixel / (double)UINT16_MAX;
				sigma += (val - mean) * (val - mean);
			}
		}
		sigma /= count;
		return sigma;
	}

	Filter getGaussianFilter(int height, int width, int r, double sigma)
	{
		Filter gauss;

		gauss.size.h = height;
		gauss.size.w = width;

		gauss.mat = new double* [width];
		for (size_t x = 0; x < width; ++x)
			gauss.mat[x] = new double[height];

		double sum = 0;
		for (int i = 0; i < 2 * r; ++i)
		{
			for (int j = 0; j < 2 * r; ++j)
			{
				gauss.mat[i][j] = exp(-((i - r) * (i - r) + (j - r) * (j - r)) / (2 * sigma * sigma));
				sum += gauss.mat[i][j];
			}
		}

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				gauss.mat[y][x] /= sum;
			}
		}
		return gauss;
	}
};
