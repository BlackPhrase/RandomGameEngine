
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <sys/time.h>
#include <stdexcept>
#include <sstream>

namespace sizzUtil
{
	uint64_t RoundDBL( double num );
	
	uint32_t CurTimeSec();
	uint64_t CurTimeMilli();
	uint64_t CurTimeNano();
	
	template<typename T, typename... Args>
	void ssprintf(std::stringstream &ss, const char* s, const T &value, Args... args);
	void ssprintf(std::stringstream &ss, const char* s);
}

inline uint64_t sizzUtil::RoundDBL( double num )
{
	return static_cast<uint64_t>(num + 0.5);
}

inline uint32_t sizzUtil::CurTimeSec()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec;
}

inline uint64_t sizzUtil::CurTimeMilli()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec*1000 + t.tv_nsec / 1000000;
}

inline uint64_t sizzUtil::CurTimeNano()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return t.tv_sec*1000000000 + t.tv_nsec;
}
 
template<typename T, typename... Args>
inline void sizzUtil::ssprintf(std::stringstream &ss, const char* s, const T &value, Args... args)
{
    while (*s)
	{
        if (*s == '%' && *(++s) != '%')
		{
            ss << value;
            ssprintf(ss, s, args...); // call even when *s == 0 to detect extra arguments
            return;
        }
        ss << *s++;
    }
    throw std::logic_error("extra arguments provided to printf");
}

inline void sizzUtil::ssprintf(std::stringstream &ss, const char* s)
{
	while (*s)
	{
		if (*s == '%' && *(++s) != '%')
			throw std::runtime_error("invalid format string: missing arguments");
		ss << *s++;
	}
}

#endif // UTILS_H
