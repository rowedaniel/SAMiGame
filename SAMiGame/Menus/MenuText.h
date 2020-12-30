#pragma once
#include "../stdafx.h"
#include "MenuItem.h"


class MenuText : public MenuItem
{

public:
	enum Action { SwapMenu = 0 };

	MenuText();
	~MenuText();

	void load(std::istream& file); // TODO: make this work via istringstring
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void updatePos(sf::Vector2f pos);

protected:

	sf::Font font; // TODO put this into a resource managing object
	sf::Text text;


	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		int fontSize = 10;
		std::string text = "";
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	void drawSubitem(sf::RenderWindow & window, sf::Text & item, sf::FloatRect & boundBox);
};
