#pragma once
#include "stdafx.h"
#include "Menus/MenuManager.h"


class Game
{

public:
	static void start();

private:
	static bool isExiting();
	static void gameLoop();

	static void showMenu();
	static sf::Vector2f screenToViewCoords(sf::Vector2f pos);


	enum GameState { Uninitialized, Paused, Playing, Exiting };
	
	static GameState gameState;
	static sf::RenderWindow mainWindow;
	static sf::View mainView;
	static sf::Clock clock;

	static MenuManager mainMenu;

	static const int orig_window_width = 512;
	static const int orig_window_height = 384;
	
};
