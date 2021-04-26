#include "iostream"
#include "chrono"
#include "eliimageprocessing.h"


// class for perfomance measurement
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	~Timer ()
	{
		std::cout << "Time elapsed: " << elapsed() << std::endl;
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "Введите имя файлов изображений!!" << std::endl;;
	}
	EliImageHandler handler;
	EliImageProcessing eliproc(handler);
	{
		Timer t;
		eliproc.divisionPxels(argv[1], argv[2], argv[3]);
	}
	{
		Timer t;
		eliproc.EliImageProcessing::divisionPxelsMultiThread(argv[1], argv[2], argv[3]);
	}
	return 0;
}
