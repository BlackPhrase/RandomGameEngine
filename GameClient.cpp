
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
}

void CGameClient::Shutdown()
{
}

// called by the engine when a key is pressed or released
bool CGameClient::KeyEvent()
{
	//sizzLog::LogDebug( "key event" );
	sizzLog::LogInfo( "average frame time: %", m_pEngine->GetAverageFrameTime() );
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
	