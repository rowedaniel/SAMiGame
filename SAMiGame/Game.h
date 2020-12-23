#pragma once
#include "stdafx.h"


class Game
{

public:
	static void start();

private:
	static bool isExiting();
	static void gameLoop();

	static void showMenu();


	enum GameState { Uninitialized, Paused, ShowingMenu, Playing, Exiting };
	
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	
};
