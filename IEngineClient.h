
#ifndef I_ENGINE_CLIENT_H
#define I_ENGINE_CLIENT_H

#include "mathutil.h"
#include <string>

//
// this is the interface of the engine to the game client
//
class IEngineClient
{
public:
	virtual float		GetScreenAspectRatio() const = 0;
	virtual void		GetScreenSize( uint32_t &width, uint32_t &height ) const = 0;
	
	// converts the 0.0f-1.0f range of x,y to a point_t in terms of pixels on the screen
	virtual void		NormalizedToScreenRes( float in_x, float in_y, point_t &out ) const = 0;
	
	virtual uint64_t	GetEngineTime() const = 0;
	
	virtual bool		SupportsDBX() const = 0;
	
	virtual void		ProcessWindowEvents() const = 0;
	
	virtual void		SetFps( uint32_t desired_fps ) = 0;
	virtual uint32_t	GetFps() const = 0;
	virtual double		GetAverageFrameTime() const = 0;
	
	virtual void		DrawText( float x, float y, const std::string &text ) const = 0;
};

//
// This is the interface of the game client to the engine
//
class IGameClient
{
public:
	virtual void	Init( IEngineClient *pEngine ) = 0;
	virtual void	Shutdown() = 0;

	// called by the engine when a key is pressed or released
	virtual bool	KeyEvent() = 0;
	
	// called by the engine when the next frame should be run on the client
	virtual void	Frame() = 0;
};

#endif // I_ENGINE_CLIENT_H
