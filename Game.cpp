
#include "Game.h"

#include "logger.h"
#include "utils.h"

#include <sys/time.h>

CGame::CGame(CXLibWindow &window):
	m_window(window),
	m_bExit(false)
{
	m_window.SetEventHandler(this);
}

CGame::~CGame()
{
	m_window.SetEventHandler(NULL);
}

void CGame::Run()
{
	while (!m_bExit)
	{
		Frame();
		
		m_window.ProcessEvents();
	}
}

void CGame::HandleEvent( const XEvent &event )
{
	switch (event.type)
	{
		case Expose:
			{
				sizzLog::LogDebug( "received Expose event" );
			}
			break;
		case ButtonPress:
			{
				sizzLog::LogDebug( "received ButtonPress event" );
			}
			break;
		case KeyPress:
			{
				sizzLog::LogDebug( "received KeyPress event" );
				m_bExit = true;
			}
			break;
		default:
			break;
	}
}

void CGame::DisplaySplashScreen()
{
	
}

void CGame::Frame()
{
	uint64_t time = sizzUtils::CurTimeMilli();
	
	static uint64_t oldtime = time;
	
	if ( time >= oldtime + 1000 )
	{
		sizzLog::LogInfo("tick");
		oldtime = time;
	}
}
