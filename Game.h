
#ifndef GAME_H
#define GAME_H

#include "XLibWindow.h"

class CGame: public IXLibEventHandler
{
public:
	CGame( CXLibWindow &window );
	~CGame();
	
	void Run();
	
	virtual void HandleEvent( const XEvent &event );
	
private:
	void DisplaySplashScreen();
	
	void Frame();
	
private:
	CXLibWindow &m_window;
	bool m_bExit;
};

#endif // GAME_H