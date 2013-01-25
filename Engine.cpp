
#include "Engine.h"

#include "Entity.h"
#include "Renderable.h"
#include "Physics.h"
#include "logger.h"

CEngine::CEngine( CXLibWindow &window, IGameServer *pGameServer, IGameClient *pGameClient ):
	m_window(window),
	m_pServer(pGameServer),
	m_pClient(pGameClient),
	m_flCurTime(0.0),
	m_flLastServerFrame(0.0),
	m_flServerTimeAccumulator(0.0),
	m_flNextClientFrameTime(0.0),
	m_flDesiredFrameTime(1000.0/60.0),
	m_nEntities(0),
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
	
	for ( CEntity *pEnt : m_entityList )
	{
		delete pEnt;
	}
	
	m_window.SetEventHandler(NULL);
}

void CEngine::Run()
{
	m_pServer->GameStart();
	while (!m_bQuit)
	{
		m_window.ProcessEvents();
		
		ClientFrame();
	}
	m_pServer->GameEnd();
}

void CEngine::ClientFrame()
{
	double cur_time = sizzUtil::CurTimeMilli();
	
	if ( cur_time >= m_flNextClientFrameTime )
	{
		m_FpsSampler.TakeSample(cur_time - m_flNextClientFrameTime);
		
		m_flNextClientFrameTime = cur_time + m_flDesiredFrameTime;
		
		ServerFrame();
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
	const double SERVER_FRAME_DT = 1.0 / 100.0;
	
	double time_now = sizzUtil::CurTimeSec();
	double frame_time = time_now - m_flLastServerFrame;
	if (frame_time > 0.250)
	{
		frame_time = 0.250;
	}
	m_flLastServerFrame = time_now;

	m_flServerTimeAccumulator += frame_time;
	
	while ( m_flServerTimeAccumulator >= SERVER_FRAME_DT )
	{
		m_pServer->GameFrame(SERVER_FRAME_DT);
		m_flCurTime += SERVER_FRAME_DT;
		m_flServerTimeAccumulator -= SERVER_FRAME_DT;
	}
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

void CEngine::GetOnScreenRenderables( std::vector<renderableContext_t> &renderables ) const
{
	renderables.clear();
	for ( CEntity *pEntity : m_entityList )
	{
		if (pEntity && pEntity->GetGraphicsComponent() && m_pServer->IsInViewBounds(pEntity))
		{
			renderableContext_t context = { Physics::GetPosition(pEntity), *pEntity->GetGraphicsComponent() };
			renderables.emplace_back(context);
		}
	}
}

// ===================================================
// Server Interface
// ===================================================

uint32_t CEngine::CreateEntity( CEntity *pEntToInsert )
{
	if (pEntToInsert)
	{
		for ( uint32_t i = 0; i < m_entityList.size(); ++i )
		{
			CEntity *pEnt = m_entityList[i];
			if (!pEnt)
			{
				pEnt = pEntToInsert;
				++m_nEntities;
				return i;
			}
		}
		
		m_entityList.emplace_back( pEntToInsert );
		++m_nEntities;
		return m_entityList.size() - 1;
	}
	else
	{
		return 0xffff;
	}
}

void CEngine::RemoveEntity( uint32_t index )
{
	CEntity *pEnt = m_entityList[index];
	if (pEnt)
	{
		delete pEnt;
		pEnt = NULL;
		--m_nEntities;
	}
}

CEntity *CEngine::GetEntity( uint32_t index )
{
	if (index < m_entityList.size())
	{
		return m_entityList[index];
	}
	return NULL;
}

uint32_t CEngine::GetNumEntites() const
{
	return m_nEntities;
}

double CEngine::GetEngineTime() const
{
	return m_flCurTime;
}
