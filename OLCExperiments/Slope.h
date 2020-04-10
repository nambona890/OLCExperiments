#pragma once
class Slope
{
private:
	float start, end, step, current;
public:
	Slope(float s, float e, int steps)
	{
		current = s; start = s; end = e;
		step = (e - s) / (float)steps;
	}
	void Step()
	{
		current += step;
	}
	float Get()
	{
		return current;
	}
	float GetS()
	{
		return start;
	}
	float GetE()
	{
		return end;
	}
};