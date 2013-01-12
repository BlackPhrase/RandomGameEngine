
#include "XLibWindow.h"
#include "Game.h"

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
