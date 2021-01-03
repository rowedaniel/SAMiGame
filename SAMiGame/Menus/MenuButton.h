#pragma once
#include "../stdafx.h"
#include "MenuText.h"
#include "MenuItem.h"


class MenuButton : public MenuText
{

public:
	enum Action { SwapMenu = 0};

	MenuButton();
	~MenuButton();

	void load(std::istream& file);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void checkMouseDown(sf::Vector2f pos);
	std::tuple<int, int> checkMouseUp(sf::Vector2f pos);

private:
	MenuButton::Action actionType;
	int actionArg;

	bool selected = false;


	struct LoadInfo
	{
	public:
		MenuText::LoadInfo oldInfo;
		int actionType = 0;
		int actionArg = 0;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
};
