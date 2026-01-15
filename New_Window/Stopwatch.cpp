#include "Stopwatch.h"
#include <chrono>
#include <thread>

float Stopwatch::Get_Duration_In_Second() const
{
	return _second;
}

void Stopwatch::Standing_By()
{
	using namespace std::chrono;
	std::this_thread::sleep_until(_last + _intervall);
	_second = Peek().count();
	Stamp();
}

void Stopwatch::Set_Interval(float second)
{
	_intervall = std::chrono::duration<float>{ second };
}

std::chrono::duration<float> Stopwatch::Peek() const
{
	using namespace std::chrono;
	auto now = steady_clock::now();
	return now - _last;
}

void Stopwatch::Stamp()
{
	_last = std::chrono::steady_clock::now();
}

