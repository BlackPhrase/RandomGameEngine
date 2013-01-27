
#include "GameClient.h"
#include "Graphics.h"
#include "Renderable.h"
#include "logger.h"

// for NULL
#include <string.h>

CGameClient::CGameClient( CGraphicsEngine *pGraphics ):
	m_pEngine(NULL),
	m_pGraphics(pGraphics)
{
}

CGameClient::~CGameClient()
{
}

void CGameClient::Init( IEngineClient *pEngine )
{
	m_pEngine = pEngine;
	m_pEngine->SetFps(50);
	m_pEngine->SetPowerSaving(true);
}

void CGameClient::Shutdown()
{
}

// called by the engine when a key is pressed or released
bool CGameClient::KeyEvent( KeySym key, bool bPressed )
{
	switch (key)
	{
		case XK_d:
		case XK_D:
			{
				m_pEngine->ServerCommand( bPressed ? "+right" : "-right" );
			}
			break;
		case XK_a:
		case XK_A:
			{
				m_pEngine->ServerCommand( bPressed ? "+left" : "-left" );
			}
			break;
		case XK_w:
		case XK_W:
			{
				m_pEngine->ServerCommand( bPressed ? "+up" : "-up" );
			}
			break;
		case XK_s:
		case XK_S:
			{
				m_pEngine->ServerCommand( bPressed ? "+down" : "-down" );
			}
			break;
		case XK_space:
			{
				m_pEngine->ServerCommand( bPressed ? "+shoot" : "-shoot" );
			}
			break;
		case XK_KP_Add:
			{
				m_pEngine->SetFps(50);
			}
			break;
		case XK_KP_Subtract:
			{
				m_pEngine->SetFps(30);
			}
			break;
		default:
			break;
	}
	return false;
}

// called by the engine when the next frame should be run on the client
void CGameClient::Frame()
{
	m_pGraphics->BeginFrame();
	
	{
		std::shared_ptr< std::vector<renderableContext_t> > pRenderables(new std::vector<renderableContext_t>());
		m_pEngine->GetOnScreenRenderables(*pRenderables);
		m_pGraphics->QueueRenderables(pRenderables);
	}
	{
		double avg_frame_time = m_pEngine->GetAverageFrameTime();
		double fps = 1000.0 / avg_frame_time;
		
		std::stringstream ss;
		sizzUtil::ssprintf(ss, "fps: % (% ms)", fps, avg_frame_time );
		m_pGraphics->DrawText(0.0f, 0.0f, ss.str());
	}
	
	m_pGraphics->EndFrame();
}
	