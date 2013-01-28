
#ifndef GAME_SERVER_H
#define GAME_SERVER_H

#include "IEngineServer.h"
#include "BulletSpawner.h"
#include <vector>

class C2DViewBounds;
class CHelicopter;
class CBullet;

class CGameServer: public IGameServer, public IBulletSpawner
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
	virtual void SpawnBullet( CBullet *pBullet );
	virtual double EngineTime();
	
private:
	void CreateQueuedEnts();
	
private:
	C2DViewBounds *GetViewBoundsEnt();
	CHelicopter *GetHelicopter();
	
	bool IsColliding( const CEntity *pEnt1, const CEntity *pEnt2 );
	void HandleCollision( CEntity *pEnt1, CEntity *pEnt2 );
	
	void CheckSpawnBuilding();
	void CheckHeliFireBullet();
	
	bool IsToTheRightOfViewBounds( CEntity *pEnt );
	
private:
	IEngineServer *m_pEngine;
	uint32_t m_entViewBounds;
	uint32_t m_entHelicopter;
	
	std::vector<CEntity*> m_entCreateQueue;
	
	double m_flLastBuildingWidth;
	double m_flViewPosOnLastBuildingSpawn;
	double m_flLastFireBullet;
	
	bool m_bGameOver;
	bool m_bViewMoving;
	bool m_bFiringBullets;
	
};

#endif // GAME_SERVER_H
