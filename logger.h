
#ifndef SIZZ_LOGGER_H
#define SIZZ_LOGGER_H

#include <sstream>
#include <iostream>

#include "utils.h"

#define COLOUR_BLACK	0
#define COLOUR_RED		1
#define COLOUR_GREEN	2
#define COLOUR_YELLOW	3
#define COLOUR_BLUE		4
#define COLOUR_MAGENTA	5
#define COLOUR_CYAN		6
#define COLOUR_WHITE	7

#define COUT_COLOUR( colour ) "\33[" << 30+(colour) << "m"
#define COUT_RESTORE_COLOUR "\033[0m"

namespace sizzLog
{
	template<typename... Args>
	void LogDebug( const char *s, Args... args );
	
	template<typename... Args>
	void LogInfo( const char *s, Args... args );
	
	template<typename... Args>
	void LogWarning( const char *s, Args... args );
	
	template<typename... Args>
	void LogError( const char *s, Args... args );
}

template<typename... Args>
inline void sizzLog::LogDebug( const char *s, Args... args )
{
#ifndef NDEBUG
	using namespace std;
	stringstream ss;
	sizzUtil::ssprintf(ss, s, args...);
	cout << COUT_COLOUR(COLOUR_GREEN) << "Debug: " << ss.rdbuf() << COUT_RESTORE_COLOUR << endl;
#endif
}

template<typename... Args>
inline void sizzLog::LogInfo( const char *s, Args... args )
{
	using namespace std;
	stringstream ss;
	sizzUtil::ssprintf(ss, s, args...);
	cout << COUT_COLOUR(COLOUR_WHITE) << "Info: " << ss.rdbuf() << COUT_RESTORE_COLOUR << endl;
}

template<typename... Args>
inline void sizzLog::LogWarning( const char *s, Args... args )
{
	using namespace std;
	stringstream ss;
	sizzUtil::ssprintf(ss, s, args...);
	cout << COUT_COLOUR(COLOUR_YELLOW) << "Warning: " << ss.rdbuf() << COUT_RESTORE_COLOUR << endl;
}

template<typename... Args>
inline void sizzLog::LogError( const char *s, Args... args )
{
	using namespace std;
	stringstream ss;
	sizzUtil::ssprintf(ss, s, args...);
	cout << COUT_COLOUR(COLOUR_RED) << "Error: " << ss.rdbuf() << COUT_RESTORE_COLOUR << endl;
}

#endif // SIZZ_LOGGER_H
