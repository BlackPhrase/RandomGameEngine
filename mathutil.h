
#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include <stdint.h>
#include <vector>

typedef struct uint_point_s
{
	uint32_t m_x;
	uint32_t m_y;
} uint_point_t;

class CAverageValueSampler
{
public:
	CAverageValueSampler():
		m_samples(32, 0.0),
		m_runningTotal(0.0),
		m_iCurSample(0)
	{
	}

	void TakeSample( double sample )
	{
		m_iCurSample %= 32;
		
		m_runningTotal -= m_samples[m_iCurSample];
		m_runningTotal += sample;
		
		m_samples[m_iCurSample++] = sample;
	}
	
	double GetAverage() const
	{
		return m_runningTotal / 32.0;
	}
	
private:
	std::vector<double> m_samples;
	double m_runningTotal;
	uint8_t m_iCurSample;
};

#endif // MATH_UTIL_H
