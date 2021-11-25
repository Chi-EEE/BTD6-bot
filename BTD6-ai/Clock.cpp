#include "Clock.h"

#include <chrono>
#include <thread>
void Clock::wait(const float t_time)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast <int> (t_time * 1000)));
}
