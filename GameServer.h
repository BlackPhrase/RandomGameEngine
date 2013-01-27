
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "IEngineServer.h"

class C2DViewBounds;
class CHelicopter;

class CGameServer: public IGameServer
{
public:
	CGameServer();
	~CGameServer();

	virtual uint32_t	Init( IEngineServer *pEngine );
	virtual void	Shutdown();

	virtual void	GameStart();
	virtual void	GameEnd();

	// called each frame by the engine
	virtual void	GameFrame( double dt );
	
	virtual bool	IsInViewBounds( CEntity *pEntity );
	
	virtual void	ReceiveCommand( const std::string &command );
	
private:
	C2DViewBounds *GetViewBoundsEnt();
	CHelicopter *GetHelicopter();
	
	bool IsColliding( const CEntity *pEnt1, const CEntity *pEnt2 );
	void HandleCollision( CEntity *pEnt1, CEntity *pEnt2 );
	
	void CheckSpawnBuilding();
	
private:
	IEngineServer *m_pEngine;
	uint32_t m_entViewBounds;
	uint32_t m_entHelicopter;
	bool m_bGameOver;
};

#endif // GAME_SERVER_H
