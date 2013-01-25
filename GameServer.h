
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "IEngineServer.h"

class C2DViewBounds;

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
	virtual void	GameFrame( double dt );
	
	virtual bool	IsInViewBounds( CEntity *pEntity );
	
private:
	C2DViewBounds *GetViewBoundsEnt();
	bool IsColliding( const CEntity *pEnt1, const CEntity *pEnt2 );
	
private:
	IEngineServer *m_pEngine;
	uint32_t m_entViewBounds;
};

#endif // GAME_SERVER_H
