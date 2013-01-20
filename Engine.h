
#ifndef ENGINE_H
#define ENGINE_H

#include "utils.h"

#include "XLibWindow.h"

#include "IEngineServer.h"
#include "IEngineClient.h"

#include "mathutil.h"

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
	virtual void HandleEvent( const XEvent &event );

	// ====================
	// client interface
	// ====================
public:
	virtual float		GetScreenAspectRatio() const;
	virtual void		GetScreenSize( uint32_t &width, uint32_t &height ) const;

	virtual double		GetEngineTime() const;

	virtual void		ProcessWindowEvents() const;

	virtual void		SetFps( uint32_t desired_fps );
	virtual uint32_t	GetFps() const;
	virtual double		GetAverageFrameTime() const;

	virtual void		SetPowerSaving( bool bEnable );

	// ====================
	// server interface
	// ====================

private:
	CXLibWindow	&m_window;
	IGameServer	*m_pServer;
	IGameClient	*m_pClient;

	// time of this frame
	double		m_flCurTime;
	
	double		m_flServerTimeAccumulator;

	double		m_flNextClientFrameTime;
	double		m_flDesiredFrameTime;
	
	CAverageValueSampler m_FpsSampler;

	bool		m_bPowerSaving;
	bool		m_bQuit;
};

#endif // ENGINE_H
