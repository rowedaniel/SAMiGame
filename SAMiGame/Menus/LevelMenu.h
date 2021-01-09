#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "CharacterButton.h"

class LevelMenu : MenuItem
{
public:
	LevelMenu();
	LevelMenu(float x, float y);
	~LevelMenu();
	
	void load(std::istream& file);
	void draw(sf::RenderWindow & window);

	void updatePos(sf::Vector2f pos);
	void updateItemPos();

private:

	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		int extraLines = 0;
		int numberOfCharacters = 0; // 4, normally?
		std::list<std::string> characterText;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);

	std::list<CharacterButton> characters;
};