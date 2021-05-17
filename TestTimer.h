#pragma once
#include <chrono>
#include <iostream>
// class for perfomance measurement
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_start;

public:
	Timer()
		: m_start(clock_t::now())
	{}

	~Timer()
	{
		std::cout << "Time elapsed: " << elapsed() << std::endl;
	}

	void reset()
	{
		m_start = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_start).count();
	}
};