#pragma once
#include "../stdafx.h"


class MenuItem
{

public:
	MenuItem();
	~MenuItem();

	void load(std::istream& file);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	int getWidth();
	int getHeight();
	void isLoaded();
	void updatePos(sf::Vector2f pos);


protected:
	bool loaded = false;

	sf::Texture texture;// TODO: move all textures to a resource manager of some sort

	// position and size go in here:
	sf::FloatRect boundingBox;

	// subitems (border, background, etc.
	const int borderSpriteSize = 8;
	sf::Sprite tlCorner;
	sf::Sprite blCorner;
	sf::Sprite trCorner;
	sf::Sprite brCorner;
	sf::Sprite tEdge;
	sf::Sprite bEdge;
	sf::Sprite lEdge;
	sf::Sprite rEdge;
	sf::RectangleShape background;



	// TODO: put parts of this into parent class for both menu buttons and menus. Maybe two structs, because the aspects are different?
	struct LoadInfo
	{
	public:
		int width = 0;
		int height = 0;
		int red = 0;
		int green = 0;
		int blue = 0;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	void drawBackground(sf::RenderWindow & window, sf::FloatRect boundBox);
	void drawBorder(sf::RenderWindow & window, sf::FloatRect boundBox);
	void drawSubitem(sf::RenderWindow & window, sf::Sprite & item, sf::FloatRect & boundBox);
	void drawSubitem(sf::RenderWindow & window, sf::RectangleShape & item, sf::FloatRect & boundBox);
};
