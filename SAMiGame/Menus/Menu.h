#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "MenuButton.h"


class Menu : public MenuItem
{

public:
	enum Direction { Left = 0, Right = 1, Up = 2, Down = 3 };

	Menu(float x, float y);
	~Menu();

	void load(std::istream& file);
	void draw(sf::RenderWindow & window);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void updatePos(sf::Vector2f pos);
	void updateItemPos();


private:
	Menu::Direction direction;
	sf::Texture texture;// TODO: move all textures to a resource manager of some sort

	std::list<Menu> menus;
	std::list<MenuButton> buttons;



	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		int numberOfButtons = 0;
		int numberOfSubmenus = 0;
		int extraLines = 0;
		int dir;
		std::list<std::string> buttonText;
		std::list<std::string> submenuText;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
};
