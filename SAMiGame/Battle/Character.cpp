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

	std::string characterFilename;
	if (playerOwned) {
		characterFilename = "";
	}
	loadTextureData();
}

void Character::draw(sf::RenderWindow & window, sf::FloatRect boundBox, sf::Time elapsedTime)
{
	animationTime += elapsedTime;
	while (animationTime > timestep) {
		animationTime -= timestep;
		animationRect.left += animationRect.width;
		if (animationRect.left >= texture.getSize().x) {
			animationRect.left = 0;
			animationRect.top += animationRect.height;
			if (animationRect.top >= texture.getSize().y) {
				animationRect.top = 0;
			}
		}
		sprite.setTextureRect(animationRect);
	}


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
	/*std::cout << "loading texture: data/images/sprites/characters/" + std::to_string(type) + ".png" << std::endl;
	if (!newtexture.loadFromFile("data/images/sprites/characters/" + std::to_string(type) + ".png")) {
		// error
		std::cout << "error loading image file for character" << std::endl;
		return;
	}
	sprite.setTexture(newtexture);
	sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));*/
	sprite = sf::Sprite(texture, animationRect);
	sprite.setPosition(sf::Vector2f(100.0f, 100.0f));

	loaded = true;
}
