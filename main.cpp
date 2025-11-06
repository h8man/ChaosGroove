#if defined(_WIN32)
#include <windows.h>
#elif defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#else
#include <unistd.h>
#endif

#include <vector>
#include <list>
#include <sstream>
#include "time.h"
#include "game.hpp"
#include "log.hpp"

using namespace std;



int WINAPI WinMain(IN HINSTANCE hInstance,
	IN HINSTANCE hPrevInstance,
	IN LPSTR lpCmdLine,
	IN int nShowCmd)
{

	// Do first time setup stuff, quitting on error.
	if (!first_time_setup()) return 0;

	game.exit = false;

start:

	if (game.exit) goto end_game;

	// We always start with the title screen.
	game.stage = GAME_MENUS;//GAME_INGAME;

	// Ingame Loop
	// -----------

	if (game.stage == GAME_MENUS)
	{
		menu_loop();
		if (game.exit) goto end_game;
	}

	game.stage = GAME_INGAME;

	if (game.stage == GAME_INGAME)
	{
		ingame_loop();
	}

	if (!game.exit) goto start;

end_game:

	log("Exiting Game..");
	do_exit_cleanup();

	return 0;
}

void _sleep(int milliseconds)
{
#if defined(__EMSCRIPTEN__)
	//void WaitTime(double seconds);     
	// Web builds cannot block — simulate with timer loop
	double start = GetTime();
	while ((GetTime() - start) * 1000.0f < milliseconds)
	{
		
	}

#elif defined(_WIN32)
	Sleep((DWORD)(milliseconds));

#else
	usleep((useconds_t)(milliseconds * 1000.0f));
#endif
}