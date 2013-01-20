
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <sys/time.h>
#include <stdexcept>
#include <sstream>

namespace sizzUtil
{
	uint64_t RoundDBL( double num );
	uint32_t RoundFlt( float num );
	
	double CurTimeSec();
	double CurTimeMilli();
	
	template<typename T, typename... Args>
	void ssprintf(std::stringstream &ss, const char* s, const T &value, Args... args);
	void ssprintf(std::stringstream &ss, const char* s);
}

inline uint64_t sizzUtil::RoundDBL( double num )
{
	return static_cast<uint64_t>(num + 0.5);
}

inline uint32_t sizzUtil::RoundFlt( float num )
{
	return static_cast<uint32_t>(num + 0.5);
}

inline double sizzUtil::CurTimeSec()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	return static_cast<double>(t.tv_sec) + static_cast<double>(t.tv_nsec)/1000000000.0;
}

inline double sizzUtil::CurTimeMilli()
{
	timespec t;
	clock_gettime(CLOCK_REALTIME, &t);
	
	double cur_time = static_cast<double>(t.tv_sec)*1000.0 + static_cast<double>(t.tv_nsec)/1000000.0;
	
	return cur_time;
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
