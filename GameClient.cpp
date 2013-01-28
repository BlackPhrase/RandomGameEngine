
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
	m_pEngine->SetFps(120);
	m_pEngine->SetPowerSaving(true);
	m_pEngine->PauseGame();
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
				m_pEngine->SetFps(120);
			}
			break;
		case XK_KP_Subtract:
			{
				m_pEngine->SetFps(60);
			}
			break;
		case XK_f:
		case XK_F:
			{
				static bool toggle = false;
				if (bPressed)
				{
					if (toggle)
					{
						m_pEngine->PauseGame();
						toggle = false;
					}
					else
					{
						m_pEngine->UnpauseGame();
						toggle = true;
					}
				}
			}
			break;
		case XK_bracketright:
			{
				m_pEngine->ServerCommand( "speed_inc" );
			}
			break;
		case XK_bracketleft:
			{
				m_pEngine->ServerCommand( "speed_dec" );
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
	if (!m_pEngine->IsPaused())
	{
		std::shared_ptr< std::vector<renderableContext_t> > pRenderables(new std::vector<renderableContext_t>());
		m_pEngine->GetOnScreenRenderables(*pRenderables);
		m_pGraphics->QueueRenderables(pRenderables);
	}
	else
	{
		ShowSplash();
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

void CGameClient::ShowSplash()
{
	m_pGraphics->DrawText(0.3f, 0.1f, "WELCOME TO THE GAME");
	m_pGraphics->DrawText(0.2f, 0.15f, "Controls:");
	
	m_pGraphics->DrawText(0.2f, 0.2f, "F: Pause and show this screen");
	m_pGraphics->DrawText(0.2f, 0.24f, "W: Fly up");
	m_pGraphics->DrawText(0.2f, 0.28f, "S: Fly down");
	m_pGraphics->DrawText(0.2f, 0.32f, "A: Fly left");
	m_pGraphics->DrawText(0.2f, 0.36f, "D: Fly right");
	m_pGraphics->DrawText(0.2f, 0.40f, "space: Shoot");
	
	m_pGraphics->DrawText(0.5f, 0.24f, "[: Decrease the scroll speed");
	m_pGraphics->DrawText(0.5f, 0.28f, "]: Increase the scroll speed");
	m_pGraphics->DrawText(0.5f, 0.36f, "-: Decrease FPS");
	m_pGraphics->DrawText(0.5f, 0.40f, "+: Increase FPS");
	
	m_pGraphics->DrawText(0.25f, 0.5f, "Shoot the turrets and get as far as you can!");
	m_pGraphics->DrawText(0.25f, 0.54f, "Press F to play the game");
	m_pGraphics->DrawText(0.15f, 0.7f, "By: Jordan Cristiano, 954");
	
	
}
	