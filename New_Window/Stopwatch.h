#pragma once
#include <chrono>

class Stopwatch
{
public:
	float Get_Duration_In_Second() const;
	void Standing_By();
	void Set_Interval(float second);
private:
	std::chrono::duration<float> Peek() const;
	void Stamp();
private:
	float _second = 0.f;
	std::chrono::duration<float> _intervall{ 1.f };
	std::chrono::time_point<std::chrono::steady_clock> _last = std::chrono::steady_clock::now();
};

