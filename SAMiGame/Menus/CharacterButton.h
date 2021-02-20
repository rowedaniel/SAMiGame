#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "../Battle/Character.h"

// TODO: make parent 'clickable' class. Also, consider adding more parent classes.
class CharacterButton : public MenuItem
{
public:
	CharacterButton();
	CharacterButton(std::vector<Character>::iterator sourceCharacter);
	~CharacterButton();

	void load();
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void setCharacterData(std::vector<Character>::iterator sourceCharacter);
	void clearCharacterData();
	bool isCharacterDataSet();

	bool contains(sf::Vector2f pos);
	void attemptSelect(sf::Vector2f pos);
	bool checkSelected(sf::Vector2f pos);
	
	void updatePos(sf::Vector2f pos);
	void updateItemPos();

	void setMatchup(int primaryMatchup, int secondaryMatchup);

	int characterType;
	std::vector<Character>::iterator characterRef;

private:

	void loadTextureData();

	sf::Texture texture; // TODO: move all textures to a resource manager of some sort
	sf::Sprite sprite;

	sf::Texture matchupTexture;
	sf::Sprite primaryMatchupSprite;
	sf::Sprite secondaryMatchupSprite;

	bool selected = false;
	bool characterDataSet = false;
};
