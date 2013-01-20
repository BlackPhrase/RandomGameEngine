
#ifndef ENGINE_H
#define ENGINE_H

#include "utils.h"

#include "XLibWindow.h"

#include "IEngineServer.h"
#include "IEngineClient.h"

#include "mathutil.h"

#include <string>

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

public:
	// IXLibEventHandler interface
	virtual void HandleEvent( const XEvent &event );

	// ====================
	// client interface
	// ====================
public:
	virtual float		GetScreenAspectRatio() const;
	virtual void		GetScreenSize( uint32_t &width, uint32_t &height ) const;

	// converts the 0.0f-1.0f range of x,y to a point_t in terms of pixels on the screen
	virtual void		NormalizedToScreenRes( float in_x, float in_y, point_t &out ) const;

	virtual uint64_t	GetEngineTime() const;

	virtual bool		SupportsDBX() const;

	virtual void		ProcessWindowEvents() const;

	virtual void		SetFps( uint32_t desired_fps );
	virtual uint32_t	GetFps() const;
	virtual double		GetAverageFrameTime() const;
	
	virtual void		DrawText( float x, float y, const std::string &text ) const;

	// ====================
	// server interface
	// ====================

private:
	CXLibWindow	&m_window;
	IGameServer	*m_pServer;
	IGameClient	*m_pClient;

	// time of this frame
	uint64_t	m_curTime;

	double		m_flNextClientFrameTime;
	double		m_flDesiredFrameTime;
	
	CAverageValueSampler m_FpsSampler;

	bool		m_bQuit;
};

#endif // ENGINE_H
