#pragma once
#include "../stdafx.h"
#include "Menu.h"
#include "LevelMenu.h"
#include "CharacterSelect.h"


class MenuManager
{
public:
	enum ActionType { Swapmenu = 0, StartLevel = 1, StartCharacterSelect = 2 };
	enum State { InMenu, InLevel, InCharacterSelect };

	MenuManager(float x, float y);
	~MenuManager();

	void load(std::istream& file);
	void draw(sf::RenderWindow & window, sf::Time elapsedTime);

	void checkMouseDown(sf::Vector2f pos);
	void checkMouseUp(sf::Vector2f pos);
	void checkMouseMove(sf::Vector2f pos);

protected:

	void executeButton(int actionType, int actionArg);

	void swapMenus(int id);
	void startLevel(int id);
	void stopLevel();
	void startCharacterSelect(int numberOfCharacters);



	float top;
	float left;

	std::vector<Menu> menus;
	std::vector<Menu>::iterator activeMenu;

	LevelMenu activeLevel;
	CharacterSelect characterSelect;
	State state = InMenu;

	struct LoadInfo
	{
	public:
		int totalNumberOfMenus = 0;
		int numberOfMenus = 0;
		int extraLines = 0;
		std::list<std::string> menuText;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
};