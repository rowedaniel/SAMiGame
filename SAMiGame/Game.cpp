#include "Game.h"



void Game::start(void)
{
	if (gameState != Uninitialized)
		return;
	
	mainWindow.create(sf::VideoMode(orig_window_width, orig_window_height, 32), "Gaem");
	mainView = sf::View(sf::FloatRect(0, 0, orig_window_width, orig_window_height));
	
	std::fstream file("data/menu.txt", std::ios::in);
	if (file.is_open()) {
		mainMenu.load(file);
	}


	gameState = Game::Playing;


	//TEST:

	while (!isExiting())
	{
		gameLoop();
	}

	mainWindow.close();
}

bool Game::isExiting()
{
	if (gameState == Game::Exiting)
		return true;
	else
		return false;
}

void Game::gameLoop()
{



	switch (gameState)
	{
	case Game::Playing:
	{
		showMenu();
		break;
	}
	}
}

void Game::showMenu()
{
	sf::Event event;
	while (mainWindow.isOpen() && gameState == Game::Playing)
	{
		// clear screen
		mainWindow.clear(sf::Color(0, 0, 0));
		
		// draw stuff
		mainMenu.draw(mainWindow, clock.getElapsedTime());
		clock.restart();
		// paint screen

		mainWindow.display();
		// events
		while (mainWindow.pollEvent(event))
		{
			switch (event.type)
			{
			case (sf::Event::Closed):
			{
				gameState = Game::Exiting;
				mainWindow.close();
				std::cout << "quitting" << std::endl;
				return;
			}
			case (sf::Event::Resized):
			{
				mainView.setSize(event.size.width * 1.0f * orig_window_height / event.size.height, 1.0f * orig_window_height);
				//mainView.setCenter(event.size.width / 2.0f, event.size.height / 2);
				mainWindow.setView(mainView);
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
	const sf::Vector2u windowSize = mainWindow.getSize();
	const float scale = 1.0f * orig_window_height / (1.0f * windowSize.y);
	// x is offset because of the way window scaling works
	const float x = (pos.x - ((windowSize.x * 1.0f) - (windowSize.y * 1.0f) * (orig_window_width * 1.0f) / (orig_window_height * 1.0f))/2 ) * scale;
	const float y = pos.y * scale;
	return sf::Vector2f(x, // this is wrong, TODO: fix it
					    y);
}

// Because C++, static member variables need to be instantiated outside of the class
Game::GameState Game::gameState = Uninitialized;
sf::RenderWindow Game::mainWindow;
sf::View Game::mainView;
sf::Clock Game::clock;
MenuManager Game::mainMenu = MenuManager(6.0f, 6.0f);