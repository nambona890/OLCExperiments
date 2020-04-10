
#include "common.h"
#include "Game.h"

int main()
{
	Game game;
	if (game.Construct(512, 480, 2, 2))
		game.Start();

	return 0;
}