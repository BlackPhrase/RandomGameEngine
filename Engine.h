
#ifndef ENGINE_H
#define ENGINE_H

#include "utils.h"

#include "XLibWindow.h"

#include "IEngineServer.h"
#include "IEngineClient.h"

#include "mathutil.h"

#include <vector>
#include <memory>

class CEntity;

class CEngine: public IEngineClient, public IEngineServer, public IXLibEventHandler
{
public:
	CEngine( CXLibWindow &window, IGameServer *pGameServer, IGameClient *pGameClient );
	~CEngine();

	void		Init();
	void		Shutdown();

	void		Run();

private:
	void		ClientFrame();
	void		ServerFrame();

	// ====================
	// IXLibEventHandler interface
	// ====================
public:
	virtual void HandleEvent( const XEvent &event, bool bQuit );

	// ====================
	// client interface
	// ====================
public:
	virtual float		GetScreenAspectRatio() const;
	virtual void		GetScreenSize( uint32_t &width, uint32_t &height ) const;

	virtual void		ProcessWindowEvents() const;

	virtual void		SetFps( uint32_t desired_fps );
	virtual uint32_t	GetFps() const;
	virtual double		GetAverageFrameTime() const;

	virtual void		SetPowerSaving( bool bEnable );
	
	virtual void		GetOnScreenRenderables( std::vector<renderableContext_t> &renderables ) const;
	
	virtual point_3d_t	GameToScreenCoords( const point_3d_t &gameCoords ) const;
	virtual point_2d_t	GameToScreenCoords( const point_2d_t &gameCoords ) const;
	virtual point_2d_t	GameToScreenCoordsNoRebase( const point_2d_t &gameCoords ) const;
	
	virtual void		ServerCommand( const std::string &command );

	// ====================
	// server interface
	// ====================
public:
	virtual uint32_t	CreateEntity( CEntity *pEntToInsert );
	virtual void		RemoveEntity( uint32_t index );
	virtual CEntity		*GetEntity( uint32_t index );
	virtual uint32_t	GetNumEntites() const;
	virtual uint32_t	IndexOfEnt( const CEntity *pEnt ) const;
	
	virtual double		GetEngineTime() const;

private:
	CXLibWindow	&m_window;
	IGameServer	*m_pServer;
	IGameClient	*m_pClient;
	
	uint32_t	m_entViewBounds;

	// time of this frame
	double		m_flCurTime;
	double		m_flLastServerFrame;
	double		m_flServerTimeAccumulator;

	double		m_flNextClientFrameTime;
	double		m_flDesiredFrameTime;
	
	CAverageValueSampler m_FpsSampler;
	
	std::vector<CEntity*> m_entityList;
	uint32_t	m_nEntities;

	bool		m_bPowerSaving;
	bool		m_bQuit;
};

#endif // ENGINE_H
