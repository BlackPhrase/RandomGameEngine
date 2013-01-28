
#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include "IEngineClient.h"

class CGraphicsEngine;

class CGameClient: public IGameClient
{
public:
	CGameClient( CGraphicsEngine *pGraphics );
	~CGameClient();

	virtual void	Init( IEngineClient *pEngine );
	virtual void	Shutdown();

	// called by the engine when a key is pressed or released
	virtual bool	KeyEvent( KeySym key, bool bPressed );

	// called by the engine when the next frame should be run on the client
	virtual void	Frame();
	
private:
	void ShowSplash();

public:
	IEngineClient *m_pEngine;
	CGraphicsEngine *m_pGraphics;
};

#endif // GAME_CLIENT_H
