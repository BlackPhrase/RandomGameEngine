
#include "GameClient.h"
#include <X11/Xlib.h>

#include "logger.h"

// for NULL
#include <string.h>

CGameClient::CGameClient():
	m_pEngine(NULL)
{
}

CGameClient::~CGameClient()
{
}

void CGameClient::Init( IEngineClient *pEngine )
{
	m_pEngine = pEngine;
	m_pEngine->SetFps(60);
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
}
	