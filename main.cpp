
#include "XLibWindow.h"
#include "Game.h"
#include "XLibGraphics.h"

int main( int argc, char *argv[] )
{
	CXLibWindow window;
	
	if (window.OpenWindow(argc, argv))
	{
		CXLibGraphics graphics;
		
		CGame game(window, graphics);

		game.Run();
		
		window.CloseWindow();
	}
}
