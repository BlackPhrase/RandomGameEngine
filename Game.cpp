
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

#define FRAME_DELTA 1000.0/60.0

void CGame::Frame()
{
	uint64_t time = sizzUtil::CurTimeMilli();
	
	static double oldtime = time;
	
	if ( time >= oldtime + FRAME_DELTA )
	{
		sizzLog::LogDebug("% tick", time);
		oldtime = time;
	}
}
