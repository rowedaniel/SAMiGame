#include "stdafx.h"
#include "Game.h"
#include "Menus/MenuManager.h"



void Game::start(void)
{
	if (_gameState != Uninitialized)
		return;
	
	_mainWindow.create(sf::VideoMode(orig_window_width, orig_window_height, 32), "Gaem");
	mainView = sf::View(sf::FloatRect(0, 0, orig_window_width, orig_window_height));
	//_mainWindow.setView(mainView);
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


	MenuManager mainMenu = MenuManager(6.0f, 6.0f);

	std::fstream file("data/menu.txt", std::ios::in);
	if (file.is_open()) {
		mainMenu.load(file);
	}

	sf::Event event;
	while (_mainWindow.isOpen() && _gameState == Game::ShowingMenu)
	{
		// clear screen
		_mainWindow.clear(sf::Color(0, 0, 0));
		// draw stuff
		mainMenu.draw(_mainWindow);
		// paint screen
		_mainWindow.display();
		// events
		while (_mainWindow.pollEvent(event))
		{
			switch (event.type)
			{
			case (sf::Event::Closed):
			{
				_gameState = Game::Exiting;
				_mainWindow.close();
				std::cout << "quitting" << std::endl;
				return;
			}
			case (sf::Event::Resized):
			{
				mainView.setSize(event.size.width * 1.0f * orig_window_height / event.size.height, 1.0f * orig_window_height);
				//mainView.setCenter(event.size.width / 2.0f, event.size.height / 2);
				_mainWindow.setView(mainView);
				break;
			}
			case (sf::Event::MouseButtonPressed):
			{
				if (event.mouseButton.button == sf::Mouse::Left) {
					mainMenu.checkMouseDown(Game::screenToViewCoords(sf::Vector2f(event.mouseButton.x + 0.0f, event.mouseButton.y + 0.0f)));
				}
				break;
			}
			case (sf::Event::MouseButtonReleased):
			{
				if (event.mouseButton.button == sf::Mouse::Left) {
					mainMenu.checkMouseUp(Game::screenToViewCoords(sf::Vector2f(event.mouseButton.x + 0.0f, event.mouseButton.y + 0.0f)));
				}
				break;
			}
			case (sf::Event::MouseMoved):
			{
				mainMenu.checkMouseMove(Game::screenToViewCoords(sf::Vector2f(event.mouseMove.x + 0.0f, event.mouseMove.y + 0.0f)));
			}
			}
		}
	}
}

sf::Vector2f Game::screenToViewCoords(sf::Vector2f pos)
{
	const sf::Vector2u windowSize = _mainWindow.getSize();
	const float scale = 1.0f * orig_window_height / (1.0f * windowSize.y);
	// x is offset because of the way window scaling works
	const float x = (pos.x - ((windowSize.x * 1.0f) - (windowSize.y * 1.0f) * (orig_window_width * 1.0f) / (orig_window_height * 1.0f))/2 ) * scale;
	const float y = pos.y * scale;
	return sf::Vector2f(x, // this is wrong, TODO: fix it
					    y);
}

// A quirk of C++, static member variables need to be instantiated outside of the class
Game::GameState Game::_gameState = Uninitialized;
sf::RenderWindow Game::_mainWindow;
sf::View Game::mainView;