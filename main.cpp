
#include "XLibWindow.h"

#include "Engine.h"
#include "GameClient.h"
#include "GameServer.h"

int main( int argc, char *argv[] )
{
	CXLibWindow window;
	
	if (window.OpenWindow(argc, argv))
	{
		CGameServer server;
		CGameClient client;

		CEngine engine(window, &server, &client);
		
		engine.Init();
		engine.Run();
		engine.Shutdown();

		window.CloseWindow();
	}
}
