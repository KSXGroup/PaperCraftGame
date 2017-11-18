#include "PaperCraft.h"
using namespace Game;
const int Game::SCREEN_WIDTH	= 1024;
const int Game::SCREEN_HEIGHT	= 768;
const std::string Game::TitleName = "Simple And Naïve Game";
SignalRouter *ctrl = new SignalRouter;

void initialize(){
	ctrl -> init();
	canvasColor = {255, 255, 255, 255};
}

int work( bool &quit )
{
	ctrl -> moveAll();
	ctrl -> drawAll();
	ctrl -> dealAll();
	if( ctrl -> keyboard[KEY_ESC] )
		quit = true;
	return 0;
}


void mousePress(){
	ctrl -> MC = 1;
}

void mouseMove(){
	ctrl -> mX = mouseX;
	ctrl -> mX = mouseY;
}

void mouseRelease(){
	ctrl -> MC = 0;	
}

void keyDown()
{
	ctrl -> keyboard[keyValue] = true;
}

void keyUp()
{
	ctrl -> keyboard[keyValue] = false;
}

void finale()
{
	delete ctrl;
}
