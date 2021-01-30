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
	static sf::Vector2f screenToViewCoords(sf::Vector2f pos);


	enum GameState { Uninitialized, Paused, ShowingMenu, Playing, Exiting };
	
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
	static sf::View mainView;

	static const int orig_window_width = 512;
	static const int orig_window_height = 384;
	
};
