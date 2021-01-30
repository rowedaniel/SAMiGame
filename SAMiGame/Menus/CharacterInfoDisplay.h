#pragma once
#include "../stdafx.h";
#include "MenuItem.h";
#include "MenuText.h";
#include "../Battle/Character.h";

// TODO: make parent 'clickable' class. Also, consider adding more parent classes.
class CharacterInfoDisplay : MenuItem
{
public:
	CharacterInfoDisplay();
	~CharacterInfoDisplay();

	void setCharacterData(std::vector<Character>::iterator sourceCharacter);
	void load(std::istream & file);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void updatePos(sf::Vector2f pos);
	void updateItemPos();

private:
	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		std::string characterNameText;
		std::string characterOtherText;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	MenuText characterName;
	MenuText characterDescription;
	MenuText characterAttack;
	
	sf::Texture typeTexture; // TODO: move all textures to a resource manager of some sort
	sf::Sprite primaryTypeSprite;
	sf::Sprite secondaryTypeSprite;


};
