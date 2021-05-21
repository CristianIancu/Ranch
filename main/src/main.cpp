#include "Game.h"

Game *game = nullptr;

int main(int argc, char *argv[])
{

	//const int FPS = 30;
	//const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width = DM.w;
    auto Height = DM.h;


	game->init("GameWindow", 1080, 2340, true);

	while (game->running())
	{

		//frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		//frameTime = SDL_GetTicks() - frameStart;
		
		//if (frameDelay > frameTime)
		//{
		//	SDL_Delay(frameDelay - frameTime);
		//}
	}

	game->clean();
	return 0;
}