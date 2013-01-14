
#ifndef GAME_H
#define GAME_H

#include "XLibWindow.h"
#include "XLibGraphics.h"

class CGame: public IXLibEventHandler
{
public:
	CGame( CXLibWindow &window, CXLibGraphics &graphics );
	~CGame();
	
	void Run();
	
	virtual void HandleEvent( const XEvent &event );
	
private:
	void DisplaySplashScreen();
	
	void Frame();
	
private:
	CXLibWindow &m_window;
	CXLibGraphics &m_graphics;
	bool m_bExit;
};

#endif // GAME_H