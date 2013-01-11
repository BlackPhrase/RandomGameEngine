
#ifndef XLIB_WINDOW_H
#define XLIB_WINDOW_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <memory>

#include "XLibGraphicsContext.h"

class IXLibEventHandler
{
public:
	virtual ~IXLibEventHandler() {}
	virtual void HandleEvent( const XEvent &event ) = 0;
};

class CNullEventHandler: public IXLibEventHandler
{
	virtual void HandleEvent(const XEvent &event) {}
};

class CXLibWindow
{
public:
	CXLibWindow();
	~CXLibWindow();
	
	void SetEventHandler( IXLibEventHandler *pHandler );
	
	bool OpenWindow( int argc, char *argv[] );
	void CloseWindow();
	
	void ResizeWindow( int width, int height );
	
	void ProcessEvents();
	
private:
	static CNullEventHandler m_NullEventHandler;

private:
	Display *m_pDisplay;
	int m_iScreen;
	Window m_Window;
	
	IXLibEventHandler *m_pEventHandler;
	
	int m_iWindowWidth;
	int m_iWindowHeight;
	
	//std::unique_ptr<CXLibGraphicsContext> m_pGraphicsContext;
};

#endif // XLIB_WINDOW_H
