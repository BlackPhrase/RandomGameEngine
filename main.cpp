
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
		engine.Run();
		engine.Shutdown();

		window.CloseWindow();
	}
}
