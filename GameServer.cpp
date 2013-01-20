
#include "GameServer.h"

#include "logger.h"

CGameServer::CGameServer():
	m_pEngine(NULL)
{
}

CGameServer::~CGameServer()
{
}

void CGameServer::Init( IEngineServer *pEngine )
{
	m_pEngine = pEngine;
}

void CGameServer::Shutdown()
{
}

void CGameServer::GameStart()
{
}

void CGameServer::GameEnd()
{
}

// called each frame by the engine
void CGameServer::GameFrame()
{
}
