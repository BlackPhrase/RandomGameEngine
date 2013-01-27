
#include "Engine.h"

#include "Entity.h"
#include "Renderable.h"
#include "Physics.h"
#include "GameEnts.h"
#include "logger.h"

#include <X11/keysymdef.h>

CEngine::CEngine( CXLibWindow &window, IGameServer *pGameServer, IGameClient *pGameClient ):
	m_window(window),
	m_pServer(pGameServer),
	m_pClient(pGameClient),
	m_entViewBounds(0),
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
	m_window.SetAutoRepeat(false);
	
	m_entViewBounds = m_pServer->Init(this);
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
	
	m_window.SetAutoRepeat(true);
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
	if (frame_time > 0.100)
	{
		frame_time = 0.100;
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

void CEngine::HandleEvent( const XEvent &event, bool bQuit )
{
	if (bQuit)
		m_bQuit = true;
	
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
				//sizzLog::LogDebug( "received KeyPress event" );
				KeySym key = m_window.GetKey(event.xkey);
				//sizzLog::LogDebug("press %", XKeysymToString(key));
				m_pClient->KeyEvent(key, true);
				//m_bQuit = true;
			}
			break;
		case KeyRelease:
			{
				//sizzLog::LogDebug( "received KeyRelease event" );
				KeySym key = m_window.GetKey(event.xkey);
				//sizzLog::LogDebug("release %", XKeysymToString(key));
				m_pClient->KeyEvent(key, false);
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
			point_3d_t pos = GameToScreenCoords(Physics::GetPosition(pEntity));
			renderableContext_t context = {pos, *pEntity->GetGraphicsComponent()};
			EShape shape = context.gcomp.GetShape();
			switch (shape)
			{
				case k_ePolygon:
					{
					}
					break;
				case k_eRectangle:
					{
						rectangle_t rect = *context.gcomp.GetRectangle();
						rect.m_max = GameToScreenCoords(rect.m_max);
						rect.m_min = GameToScreenCoords(rect.m_min);
						context.gcomp.SetRectangle(rect);
					}
					break;
				case k_eArc:
					{
					}
					break;
				default:
					break;
			}
			
			renderables.emplace_back(context);
		}
	}
}

point_3d_t CEngine::GameToScreenCoords( const point_3d_t &gameCoords ) const
{
	C2DViewBounds *pView = dynamic_cast<C2DViewBounds*>(m_entityList[m_entViewBounds]);
	point_2d_t viewPos = pView->GetPhysComponent()->Get2DPosition();

	point_2d_t fov = pView->GetFov();
	point_3d_t out = {0.0, 0.0, gameCoords.m_z};
	
	// calculates game coords with the view being the origin
	out.m_x = gameCoords.m_x - viewPos.m_x;
	out.m_y = gameCoords.m_y - viewPos.m_y;
	
	out.m_x *= (m_window.GetWindowWidth() / fov.m_x);
	out.m_y *= (m_window.GetWindowHeight() / fov.m_y);
	
	return out;
}

point_2d_t CEngine::GameToScreenCoords( const point_2d_t &gameCoords ) const
{
	point_3d_t temp = GameToScreenCoords( {gameCoords.m_x, gameCoords.m_y, 0.0} );
	return { temp.m_x, temp.m_y };
}

void CEngine::ServerCommand( const std::string &command )
{
	m_pServer->ReceiveCommand(command);
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
		m_entityList[index] = NULL;
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

uint32_t CEngine::IndexOfEnt( const CEntity *pEnt ) const
{
	if (pEnt)
	{
		// bad loop, this should be constant time in a good design
		for ( uint32_t i = 0; i < m_entityList.size(); ++i )
		{
			const CEntity *pTemp = m_entityList[i];
			if (pEnt == pTemp)
			{
				return i;
			}
		}
	}
	return 0xffff;
}

double CEngine::GetEngineTime() const
{
	return m_flCurTime;
}
