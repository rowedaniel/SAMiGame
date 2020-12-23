#include "stdafx.h"
#include "SplashScreen.h"


void SplashScreen::show(sf::RenderWindow & renderWindow)
{
	std::cout << "in SplashScreen" << std::endl;
	sf::Texture texture;
	if (!texture.loadFromFile("images/SplashScreen.bmp"))
	{
		// error
		std::cout << "AHHHHHH" << std::endl;
		return;
	}

	sf::Sprite sprite;
	sprite.setTexture(texture);

	renderWindow.draw(sprite);
	renderWindow.display();

	sf::Event event;
	while (true)
	{
		while (renderWindow.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::KeyPressed
				|| event.type == sf::Event::EventType::MouseButtonPressed
				|| event.type == sf::Event::EventType::Closed)
			{
				return;
			}
		}
	}
}