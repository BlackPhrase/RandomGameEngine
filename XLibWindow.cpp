
#include "XLibWindow.h"

#include "logger.h"

CNullEventHandler CXLibWindow::m_NullEventHandler;

CXLibWindow::CXLibWindow():
	m_pDisplay(NULL),
	m_iScreen(0),
	m_Window(0),
	m_iWindowWidth(0),
	m_iWindowHeight(0)
{
}

CXLibWindow::~CXLibWindow()
{
	CloseWindow();
}

void CXLibWindow::SetEventHandler( IXLibEventHandler *pHandler )
{
	if (pHandler)
	{
		m_pEventHandler = pHandler;
	}
	else
	{
		m_pEventHandler = &m_NullEventHandler;
	}
}

bool CXLibWindow::OpenWindow( int argc, char *argv[] )
{
	m_pDisplay = XOpenDisplay("");
	if (!m_pDisplay)
	{
		sizzLog::LogError( "Couldn't open display." );
		return false;
	}

	m_iScreen = DefaultScreen(m_pDisplay);
	
	unsigned long white = XWhitePixel( m_pDisplay, m_iScreen );
	unsigned long black = XBlackPixel( m_pDisplay, m_iScreen );
	
	XSizeHints hints =
	{
		PPosition | PSize,
		100,
		100,
		640,
		480,
	};
	
	m_Window = XCreateSimpleWindow(
		m_pDisplay,
		DefaultRootWindow(m_pDisplay),
		hints.x, hints.y,
		hints.width, hints.height,
		5,
		black,
		white );
	
	XSetStandardProperties(
		m_pDisplay,
		m_Window,
		"asdfasdf",
		"icon title",
		None,
		argv, argc,
		&hints );
		
	XSelectInput(m_pDisplay, m_Window, ButtonPressMask | KeyPressMask | ExposureMask);
		
	XMapRaised(m_pDisplay, m_Window);
	XFlush(m_pDisplay);
	
	return true;
}

void CXLibWindow::CloseWindow()
{
}

void CXLibWindow::ResizeWindow( int width, int height )
{
	m_iWindowWidth = width;
	m_iWindowHeight = height;
}

void CXLibWindow::ProcessEvents()
{
	XEvent event;
	
	int nEvents = XPending(m_pDisplay);
	while (nEvents--)
	{
		XNextEvent(m_pDisplay, &event);
		m_pEventHandler->HandleEvent(event);
	}
}
