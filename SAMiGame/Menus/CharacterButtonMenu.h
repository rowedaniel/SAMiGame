#pragma once
#include "MenuItem.h"
#include "CharacterButton.h"

class CharacterButtonMenu : public MenuItem
{
public:
	CharacterButtonMenu();
	~CharacterButtonMenu();

	void unload();
	void draw(sf::RenderWindow & window, sf::FloatRect & boundBox, sf::Time elapsedTime);
	void addCharacter(std::vector<CharacterButton>::iterator character);
	void removeCharacter();
	void updatePos(sf::Vector2f pos);
	void scrollCharacter(int numberOfCharacters);


private:

	void updateItemPos();

	void updateScroll(sf::Time elapsedTime);

	std::vector<std::vector<CharacterButton>::iterator> characterRefs;
	float characterOffset = 0.0f;
	float targetCharacterOffset = 0.0f;
	float changeSpeed = 0.0f;

	int totalCharacterCount = 0;
};