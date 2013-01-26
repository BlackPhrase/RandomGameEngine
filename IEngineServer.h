
#ifndef I_ENGINE_SERVER_H
#define I_ENGINE_SERVER_H

#include <stdint.h>

class CEntity;

//
// this is the interface of the engine to the game server
//
class IEngineServer
{
public:
	virtual uint32_t	CreateEntity( CEntity *pEntToInsert ) = 0;
	virtual void		RemoveEntity( uint32_t index ) = 0;
	virtual CEntity		*GetEntity( uint32_t index ) = 0;
	virtual uint32_t	GetNumEntites() const = 0;

	virtual double		GetEngineTime() const = 0;
};

//
// This is the interface of the game server to the engine
//
class IGameServer
{
public:
	virtual uint32_t	Init( IEngineServer *pEngine ) = 0;
	virtual void	Shutdown() = 0;

	virtual void	GameStart() = 0;
	virtual void	GameEnd() = 0;

	// called each frame by the engine
	virtual void	GameFrame( double dt ) = 0;
	
	virtual bool	IsInViewBounds( CEntity *pEntity ) = 0;
};

#endif // I_ENGINE_SERVER_H
