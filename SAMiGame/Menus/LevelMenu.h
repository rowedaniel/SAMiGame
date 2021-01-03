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


	void updateItemPos();

private:

	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);

	CharacterButton testButton;
};