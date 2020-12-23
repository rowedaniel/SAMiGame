#include "stdafx.h"
#include "Game.h"
#include "Menus/Menu.h"
#include "Menus/MenuButton.h"
#include <iostream>
#include <fstream>
#include <sstream>



void Game::start(void)
{
	if (_gameState != Uninitialized)
		return;

	_mainWindow.create(sf::VideoMode(512, 384, 32), "Gaem");
	_gameState = Game::ShowingMenu;


	//TEST:

	while (!isExiting())
	{
		gameLoop();
	}

	_mainWindow.close();
}

bool Game::isExiting()
{
	if (_gameState == Game::Exiting)
		return true;
	else
		return false;
}

void Game::gameLoop()
{



	switch (_gameState)
	{
	case Game::ShowingMenu:
	{
		showMenu();
		break;
	}
	case Game::Playing:
	{

		sf::Event currentEvent;
		while (_mainWindow.isOpen() && _gameState == Game::Playing)
		{

			// clear screen
			_mainWindow.clear(sf::Color(0, 0, 0));

			// draw stuff


			// paint screen
			_mainWindow.display();


			// events
			while (_mainWindow.pollEvent(currentEvent))
			{
				if (currentEvent.type == sf::Event::Closed)
				{
					_gameState = Game::Exiting;
					_mainWindow.close();
					break;
				}
				else if (currentEvent.type == sf::Event::KeyPressed)
				{
					switch (currentEvent.key.code)
					{
					case sf::Keyboard::Escape:
					{
						showMenu();
						break;
					}
					default:
					{
						break;
					}
					}
				}
			}
		}
		break;
	}
	}
}

void Game::showMenu()
{
	std::list<int> test[2];


	Menu menu = Menu(6, 6);

	std::fstream file;
	file.open("data/menu.txt", std::ios::in);
	if (file.is_open()) {
		menu.load(file);
	}

	sf::Event currentEvent;
	while (_mainWindow.isOpen() && _gameState == Game::ShowingMenu)
	{
		// clear screen
		_mainWindow.clear(sf::Color(0, 0, 0));
		// draw stuff
		menu.draw(_mainWindow);
		// paint screen
		_mainWindow.display();
		// events
		while (_mainWindow.pollEvent(currentEvent))
		{
			if (currentEvent.type == sf::Event::Closed)
			{
				_gameState = Game::Exiting;
				_mainWindow.close();
				break;
			}
		}
	}
}

// A quirk of C++, static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;