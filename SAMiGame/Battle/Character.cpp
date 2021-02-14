#include "Character.h"

Character::Character(int characterType) : CharacterTemplate(characterType)
{
	std::cout << "new character made of type: " << type << std::endl;
}

Character::~Character()
{
}

void Character::load(bool playerOwned)
{
	CharacterTemplate::load();


	loadTextureData();
}

void Character::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	window.draw(sprite);
}

float Character::getPrimaryMatchup(std::vector<Character>::iterator opponent)
{
	return primaryMatchups[(primary_type - opponent->primary_type+4)%4];
}

float Character::getSecondaryMatchup(std::vector<Character>::iterator opponent)
{
	return secondaryMatchups[(secondary_type - opponent->secondary_type + 5) % 5];
}

void Character::loadTextureData()
{
	sprite = sf::Sprite(texture, sf::IntRect(0, 0, 64, 64));
	sprite.setPosition(sf::Vector2f(100.0f, 100.0f));
}
