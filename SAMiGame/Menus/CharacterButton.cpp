#include "CharacterButton.h"


CharacterButton::CharacterButton()
{
	// by default, use empty sprite
	clearCharacterData();
}

CharacterButton::CharacterButton(std::vector<Character>::iterator sourceCharacter)
{
	setCharacterData(sourceCharacter);
}

CharacterButton::~CharacterButton()
{
}

void CharacterButton::load()
{
	loadTextureData();
}

void CharacterButton::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	//MenuItem::draw(window, boundBox);
	drawSubitem(window, sprite, boundBox);
	drawSubitem(window, primaryMatchupSprite, boundBox);
	drawSubitem(window, secondaryMatchupSprite, boundBox);
	//std::cout << "in characterbutton::draw, boundBox is: " << sprite.getGlobalBounds().width << ", " << sprite.getGlobalBounds().height << std::endl;
	//std::cout << "in characterbutton::draw, pos is: " << sprite.getPosition().x << ", " << sprite.getPosition().y << std::endl;
}

void CharacterButton::setCharacterData(std::vector<Character>::iterator sourceCharacter)
{
	characterRef = sourceCharacter;
	characterType = sourceCharacter->type;
	//sprite.setTextureRect(sf::IntRect(1 * 32, 0, 32, 32));
	sprite.setTextureRect(sf::IntRect(sourceCharacter->type * 32, 0, 32, 32));
	characterDataSet = true;
}

void CharacterButton::clearCharacterData()
{
	sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	characterDataSet = false;
}

bool CharacterButton::isCharacterDataSet()
{
	return characterDataSet;
}


bool CharacterButton::contains(sf::Vector2f pos)
{
	return boundingBox.contains(pos);
}

void CharacterButton::attemptSelect(sf::Vector2f pos)
{
	if (boundingBox.contains(pos)) {
		selected = true;
	}
	else {
		selected = false;
	}
}

bool CharacterButton::checkSelected(sf::Vector2f pos)
{
	if (selected && boundingBox.contains(pos)) {
		return true;
	}
	selected = false;
	return false;
}

void CharacterButton::updatePos(sf::Vector2f pos)
{
	boundingBox.left = pos.x;
	boundingBox.top = pos.y;
	updateItemPos();
}

void CharacterButton::updateItemPos()
{
	const sf::Vector2f pos = sf::Vector2f(boundingBox.left, boundingBox.top);
	sprite.setPosition(pos);
	primaryMatchupSprite.setPosition(pos);
	secondaryMatchupSprite.setPosition(pos);
}

void CharacterButton::setMatchup(int primaryMatchup, int secondaryMatchup)
{
	primaryMatchupSprite.setTextureRect(sf::IntRect(primaryMatchup * 32, 0, 32, 32));
	secondaryMatchupSprite.setTextureRect(sf::IntRect(secondaryMatchup * 32, 32, 32, 32));
}

void CharacterButton::resetScale()
{
	sprite.setScale(1.0f, 1.0f);
}

void CharacterButton::loadTextureData()
{
	//MenuItem::loadTextureData();
	

	boundingBox.width = 32;
	boundingBox.height = 32;

	// main texture/sprite

	if (!texture.loadFromFile("data/images/sprites/characterButtons.png")) {
		// error
		std::cout << "error loading image file for character buttons" << std::endl;
		return;
	}
	sprite.setTexture(texture);


	// matchup texture/sprite

	if (!matchupTexture.loadFromFile("data/images/sprites/matchups.png")) {
		// error
		std::cout << "error loading image file for character buttons" << std::endl;
		return;
	}
	primaryMatchupSprite = sf::Sprite(matchupTexture, sf::IntRect(0, 0, 32, 32));
	secondaryMatchupSprite = sf::Sprite(matchupTexture, sf::IntRect(0, 32, 32, 32));
}
