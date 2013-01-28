
#include "XLibWindow.h"

#include "Engine.h"
#include "Graphics.h"
#include "GameClient.h"
#include "GameServer.h"

int main( int argc, char *argv[] )
{
	srand(0);
	CXLibWindow window;
	
	if (window.OpenWindow(argc, argv))
	{
		CGraphicsEngine graphics(window);
		
		CGameServer server;
		CGameClient client(&graphics);
		
		CEngine engine(window, &server, &client);
		
		engine.Init();
		bool playing = true;
		while (playing)
		{
			try
			{
				engine.Run();
				playing = false;
			}
			catch( bool quit )
			{
				playing = quit;
			}
		}
		engine.Shutdown();

		window.CloseWindow();
	}
}
