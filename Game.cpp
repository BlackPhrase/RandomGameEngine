
#include "Game.h"

#include "logger.h"
#include "utils.h"

#include <sys/time.h>

#define FRAME_DELTA 1000.0/120.0

CGame::CGame(CXLibWindow &window, CXLibGraphics &graphics):
	m_window(window),
	m_graphics(graphics),
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
		m_window.ProcessEvents();

		Frame();
		
		uint64_t time = sizzUtil::CurTimeMilli();
		static double oldtime = time - FRAME_DELTA;
		
		if ( time >= oldtime + FRAME_DELTA )
		{
			sizzLog::LogInfo("render %", time);
			m_graphics.Render();
			oldtime = time;
		}
		else
		{
			double difference = oldtime+FRAME_DELTA - time;
			int32_t nano_time = difference*1000000*0.98;
			struct timespec sleep_time = {0, nano_time};
			struct timespec temp = {0, 0};

			nanosleep(&sleep_time, &temp);
		}
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

}
