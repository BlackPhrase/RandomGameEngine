
#include "XLibWindow.h"

#include "logger.h"

CNullEventHandler CXLibWindow::m_NullEventHandler;

CXLibWindow::CXLibWindow():
	m_pDisplay(NULL),
	m_iScreen(0),
	m_Window(0),
	m_pEventHandler(&m_NullEventHandler),
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
	
	m_iWindowWidth = 640;
	m_iWindowHeight = 480;
	
	XSizeHints hints =
	{
		PPosition | PSize,
		100,
		100,
		m_iWindowWidth,
		m_iWindowHeight,
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
		
	XSelectInput(m_pDisplay, m_Window, ButtonPressMask | KeyPressMask | KeyReleaseMask | ExposureMask);
	
	// register interest in the delete window message
	m_wmDeleteMessage = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(m_pDisplay, m_Window, &m_wmDeleteMessage, 1);
	
	XMapRaised(m_pDisplay, m_Window);
	XFlush(m_pDisplay);
	
	return true;
}

void CXLibWindow::CloseWindow()
{
}

void CXLibWindow::ResizeWindow( int width, int height )
{
	if ( m_iWindowWidth != width || m_iWindowHeight != height )
	{
		m_iWindowWidth = width;
		m_iWindowHeight = height;
		
		XWindowAttributes asdf;
		XGetWindowAttributes(m_pDisplay, m_Window, &asdf);
		sizzLog::LogDebug("width: %, height %", asdf.width, asdf.height);
	}
}

void CXLibWindow::ProcessEvents()
{
	XEvent event;
	
	int nEvents = XPending(m_pDisplay);
	while (nEvents--)
	{
		XNextEvent(m_pDisplay, &event);
		bool bQuit = (event.type == ClientMessage)
				&& (event.xclient.data.l[0] == m_wmDeleteMessage);
		m_pEventHandler->HandleEvent(event, bQuit);
	}
}

Pixmap CXLibWindow::CreatePixmap( float width, float height )
{
	return XCreatePixmap(m_pDisplay, m_Window, width, height, DefaultDepth(m_pDisplay, m_Window));
}

void CXLibWindow::FreePixmap( Pixmap pixmap )
{
	XFreePixmap(m_pDisplay, pixmap);
}

int CXLibWindow::GetWindowWidth() const
{
	return m_iWindowWidth;
}

int CXLibWindow::GetWindowHeight() const
{
	return m_iWindowHeight;
}
	
float CXLibWindow::GetAspectRatio() const
{
	return static_cast<float>(m_iWindowWidth) / static_cast<float>(m_iWindowHeight);
}

KeySym CXLibWindow::GetKey( const XKeyEvent &event ) const
{
	int keysyms_per_keycode_return = 0;
	KeySym *pKey = static_cast<KeySym*>(XGetKeyboardMapping(m_pDisplay,event.keycode,1,&keysyms_per_keycode_return));
	KeySym key = *pKey;
	XFree(pKey);
	return key;
}
