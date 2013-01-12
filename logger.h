
#ifndef SIZZ_LOGGER_H
#define SIZZ_LOGGER_H

#include <string>
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
	template<typename T, typename... Args>
	void LogDebug( const char *s, T value, Args... args );

	void LogDebug( const std::string &msg );

	void LogInfo( const std::string &msg );
	void LogWarning( const std::string &msg );
	void LogError( const std::string &msg );
}

template<typename T, typename... Args>
inline void sizzLog::LogDebug( const char *s, T value, Args... args )
{
#ifndef NDEBUG
	using namespace std;
	stringstream ss;
	sizzUtil::ssprintf(ss, s, value, args...);
	cout << COUT_COLOUR(COLOUR_GREEN) << "Debug: " << ss.rdbuf() << COUT_RESTORE_COLOUR << endl;
#endif
}

inline void sizzLog::LogDebug( const std::string &msg )
{
#ifndef NDEBUG
	using namespace std;
	cout << COUT_COLOUR(COLOUR_GREEN) << "Debug: " << msg << COUT_RESTORE_COLOUR << endl;
#endif
}

inline void sizzLog::LogInfo( const std::string &msg )
{
	using namespace std;
	cout << COUT_COLOUR(COLOUR_WHITE) << "Info: " << msg << COUT_RESTORE_COLOUR << endl;
}

inline void sizzLog::LogWarning( const std::string &msg )
{
	using namespace std;
	cout << COUT_COLOUR(COLOUR_YELLOW) << "Warning: " << msg << COUT_RESTORE_COLOUR << endl;
}

inline void sizzLog::LogError( const std::string &msg )
{
	using namespace std;
	cout << COUT_COLOUR(COLOUR_RED) << "Error: " << msg << COUT_RESTORE_COLOUR << endl;
}

#endif // SIZZ_LOGGER_H
