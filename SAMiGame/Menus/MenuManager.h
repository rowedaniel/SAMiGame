#pragma once
#include "../stdafx.h"
#include "Menu.h"
#include "LevelMenu.h"


class MenuManager
{
public:
	enum ActionType { Swapmenu = 0, StartLevel = 1 };

	MenuManager(float x, float y);
	~MenuManager();

	void load(std::istream& file);
	void draw(sf::RenderWindow & window);

	void checkMouseDown(sf::Vector2f pos);
	void checkMouseUp(sf::Vector2f pos);
	void executeButton(int actionType, int actionArg);
	void swapMenus(int id);
	void startLevel(int id);
	void stopLevel();
protected:
	float top;
	float left;

	std::list<Menu> menus;
	std::list<Menu>::iterator activeMenu;

	LevelMenu activeLevel;
	bool inLevel = false;

	struct LoadInfo
	{
	public:
		int numberOfMenus;
		int extraLines;
		std::list<std::string> menuText;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
};