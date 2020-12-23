#pragma once
#include "Menu.h"


class MainMenu : public Menu
{
public:
	enum MenuResult { Nothing=0, Exit=1, Play=2 };
	MenuResult show(sf::RenderWindow& window);
};
