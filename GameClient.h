
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "IEngineClient.h"

class CGameClient: public IGameClient
{
public:
	CGameClient();
	~CGameClient();

	virtual void	Init( IEngineClient *pEngine );
	virtual void	Shutdown();

	// called by the engine when a key is pressed or released
	virtual bool	KeyEvent();

	// called by the engine when the next frame should be run on the client
	virtual void	Frame();

public:
	IEngineClient *m_pEngine;
};

#endif // GAME_CLIENT_H
