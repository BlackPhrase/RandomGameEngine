
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "IEngineServer.h"

class CGameServer: public IGameServer
{
public:
	CGameServer();
	~CGameServer();

	virtual void	Init( IEngineServer *pEngine );
	virtual void	Shutdown();

	virtual void	GameStart();
	virtual void	GameEnd();

	// called each frame by the engine
	virtual void	GameFrame();
	
private:
	IEngineServer *m_pEngine;
};

#endif // GAME_SERVER_H
