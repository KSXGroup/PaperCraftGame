#include "PaperCraft.h"
using namespace Game;
const int Game::SCREEN_WIDTH	= 1024;
const int Game::SCREEN_HEIGHT	= 768;
const std::string Game::TitleName = "NOT A Simple Game Demo";
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


void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

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
