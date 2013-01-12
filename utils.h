
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <sys/time.h>

namespace sizzUtils
{
	uint64_t RoundDBL( double num );
	
	uint32_t CurTimeSec();
	uint64_t CurTimeMilli();
	uint64_t CurTimeNano();
}

inline uint64_t sizzUtils::RoundDBL( double num )
{
	return static_cast<uint64_t>(num + 0.5);
}

inline uint32_t sizzUtils::CurTimeSec()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec;
}

inline uint64_t sizzUtils::CurTimeMilli()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec*1000 + t.tv_nsec / 1000000;
}

inline uint64_t sizzUtils::CurTimeNano()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec*1000000000 + t.tv_nsec;
}

#endif // UTILS_H