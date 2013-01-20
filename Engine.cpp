
#include "Engine.h"
#include "logger.h"

CEngine::CEngine( CXLibWindow &window, IGameServer *pGameServer, IGameClient *pGameClient ):
	m_window(window),
	m_pServer(pGameServer),
	m_pClient(pGameClient),
	m_flCurTime(sizzUtil::CurTimeMilli()),
	m_flServerTimeAccumulator(0.0),
	m_flNextClientFrameTime(0.0),
	m_flDesiredFrameTime(1000.0/60.0),
	m_bPowerSaving(false),
	m_bQuit(false)
{
}

CEngine::~CEngine()
{
}
	
void CEngine::Init()
{
	m_window.SetEventHandler(this);
	
	m_pServer->Init(this);
	m_pClient->Init(this);
}

void CEngine::Shutdown()
{
	m_pClient->Shutdown();
	m_pServer->Shutdown();
	
	m_window.SetEventHandler(NULL);
}

void CEngine::Run()
{
	while (!m_bQuit)
	{
		m_flCurTime = sizzUtil::CurTimeMilli();
		
		m_window.ProcessEvents();
		
		ServerFrame();
		
		ClientFrame();
	}
}

void CEngine::ClientFrame()
{
	double cur_time = sizzUtil::CurTimeMilli();
	
	if ( cur_time >= m_flNextClientFrameTime )
	{
		m_FpsSampler.TakeSample(cur_time - m_flNextClientFrameTime);
		
		m_flNextClientFrameTime = cur_time + m_flDesiredFrameTime;
		
		m_pClient->Frame();
	}
	else if (m_bPowerSaving)
	{
		double difference = m_flNextClientFrameTime - cur_time;
		if (difference > 2.0)
		{
			int32_t nano_time = difference*1000000*0.90;
			struct timespec sleep_time = {0, nano_time};
			struct timespec temp = {0, 0};
			
			nanosleep( &sleep_time, &temp );
		}
	}
}

void CEngine::ServerFrame()
{
	static const double SERVER_FRAME_DT = 1000.0 / 60.0;
	/*
	m_flServerTimeAccumulator += frame_time;
	
	while ( m_flServerTimeAccumulator >= SERVER_FRAME_DT )
	{
		m_pServer->GameFrame();
		m_flServerTimeAccumulator -= SERVER_FRAME_DT;
	}*/
}

void CEngine::HandleEvent( const XEvent &event )
{
	switch (event.type)
	{
		case Expose:
			{
				const XExposeEvent *pEvent = &event.xexpose;
				
				m_window.ResizeWindow(pEvent->width, pEvent->height);
				
				sizzLog::LogDebug( "received Expose event %", pEvent->count );
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
				m_pClient->KeyEvent();
				//m_bQuit = true;
			}
			break;
		default:
			break;
	}
}

// ===================================================
// Client Interface
// ===================================================

float CEngine::GetScreenAspectRatio() const
{
	return m_window.GetAspectRatio();
}

void CEngine::GetScreenSize( uint32_t &width, uint32_t &height ) const
{
	width = m_window.GetWindowWidth();
	height = m_window.GetWindowHeight();
}

double CEngine::GetEngineTime() const
{
	return m_flCurTime;
}

void CEngine::ProcessWindowEvents() const
{
}

void CEngine::SetFps( uint32_t desired_fps )
{
	m_flDesiredFrameTime = (desired_fps > 0) ? (1000.0f / desired_fps) : 0.0;
}

uint32_t CEngine::GetFps() const
{
	return sizzUtil::RoundFlt( 1000.0f / m_flDesiredFrameTime );
}

double CEngine::GetAverageFrameTime() const
{
	return m_flDesiredFrameTime + m_FpsSampler.GetAverage();
}

void CEngine::SetPowerSaving( bool bEnable )
{
	m_bPowerSaving = bEnable;
}

// ===================================================
// Server Interface
// ===================================================


