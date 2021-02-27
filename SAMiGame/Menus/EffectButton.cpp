#include "EffectButton.h"

EffectButton::EffectButton()
{
}

EffectButton::~EffectButton()
{
}

void EffectButton::load()
{
	// load description stuff
	{
		std::fstream file("data/effectText.menuItem", std::ios::in);
		if (file.is_open()) {
			descriptionBg.load(file);
		}
	}
	{
		std::fstream file("data/effectText.menuItem", std::ios::in);
		if (file.is_open()) {
			descriptionText.load(file);
		}
	}



	loadTextureData();
	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
}

void EffectButton::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	drawSubitem(window, characterSprite, boundBox);
	drawSubitem(window, bgSprite, boundBox);

	if (showDescription) {

		while (boundingBox.left + descriptionOffset + descriptionBg.getWidth() + 4.0f > boundBox.left + boundBox.width) {
			descriptionOffset -= 4.0f;
			updateItemPos();
		}

		descriptionBg.draw(window, boundBox);
		descriptionText.draw(window, boundBox);
	}
}

bool EffectButton::contains(sf::Vector2f pos)
{
	return boundingBox.contains(pos);
}

void EffectButton::updatePos(sf::Vector2f pos)
{
	boundingBox.left = pos.x;
	boundingBox.top = pos.y;
	updateItemPos();
}

void EffectButton::updateItemPos()
{
	sf::Vector2f pos = sf::Vector2f(boundingBox.left, boundingBox.top);

	bgSprite.setPosition(pos);
	characterSprite.setPosition(pos);

	// set description position
	descriptionBg.updatePos(pos + sf::Vector2f(descriptionOffset, -16.0f));
	descriptionText.updatePos(pos + sf::Vector2f(descriptionOffset, -16.0f));
}

void EffectButton::setEffectData(std::list<Effect>::iterator sourceEffect)
{
	effectRef = sourceEffect;
	description = sourceEffect->getDescription();
	bgSprite.setTextureRect(sf::IntRect(sourceEffect->isPlayerOwned() * 32, 0, boundingBox.width, boundingBox.height));
	characterSprite.setTextureRect(sf::IntRect(sourceEffect->getSourceCharacterType() * 32, 0, boundingBox.width, boundingBox.height));
	descriptionText.setText(description);
}

std::string EffectButton::getDescription()
{
	return description;
}

void EffectButton::checkMouseMove(sf::Vector2f pos)
{
	if (boundingBox.contains(pos)) {
		if (showDescription) {
			return; // already displayed, don't update again
		}

		showDescription = true;
	}
	else {
		if (!showDescription) {
			return; // already not displayed, don't update again
		}
		showDescription = false;
	}
}

void EffectButton::loadTextureData()
{
	boundingBox.width = 32;
	boundingBox.height = 32;

	// TODO: consider swapping this around, for better data handling
	if (!bgTexture.loadFromFile("data/images/sprites/effects.png")) {
		// error
		std::cout << "error loading image file for effect buttons" << std::endl;
		return;
	}
	bgSprite.setTexture(bgTexture);

	if (!characterTexture.loadFromFile("data/images/sprites/characterButtons.png")) {
		// error
		std::cout << "error loading image file for effect buttons" << std::endl;
		return;
	}
	characterSprite.setTexture(characterTexture);
}
