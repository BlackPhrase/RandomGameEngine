
#include "XLibWindow.h"
#include "logger.h"

class CGame: public IXLibEventHandler
{
public:
	CGame( CXLibWindow &window );
	~CGame();
	
	void Run();
	
	virtual void HandleEvent( const XEvent &event );
	
private:
	void Frame() {}
	
private:
	CXLibWindow &m_window;
	bool m_bExit;
};

CGame::CGame(CXLibWindow &window):
	m_window(window),
	m_bExit(false)
{
	m_window.SetEventHandler(this);
}

CGame::~CGame()
{
	m_window.SetEventHandler(NULL);
}

void CGame::Run()
{
	while (!m_bExit)
	{
		Frame();
		
		m_window.ProcessEvents();
	}
}

void CGame::HandleEvent( const XEvent &event )
{
	switch (event.type)
	{
		case Expose:
			{
				sizzLog::LogDebug( "received Expose event" );
			}
			break;
		case ButtonPress:
			{
				sizzLog::LogDebug( "received ButtonPress event" );
			}
			break;
		case KeyPress:
			{
				sizzLog::LogDebug( "received KeyPress event" );
				m_bExit = true;
			}
			break;
		default:
			break;
	}
}

int main( int argc, char *argv[] )
{
	CXLibWindow window;
	
	if (window.OpenWindow(argc, argv))
	{
		CGame game(window);

		game.Run();
		
		window.CloseWindow();
	}
}
