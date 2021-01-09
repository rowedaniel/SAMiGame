#pragma once
#include "../stdafx.h";
#include "MenuButton.h";

// TODO: make parent 'clickable' class. Also, consider adding more parent classes.
class CharacterButton : public MenuItem
{
public:
	void load(std::istream& file);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void updatePos(sf::Vector2f pos);

private:
	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
	};

	sf::Texture texture; // TODO: move all textures to a resource manager of some sort
	sf::Sprite sprite;

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();
};
