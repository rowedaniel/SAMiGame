#include "Character.h"

Character::Character(int characterType) : CharacterTemplate(characterType)
{
	std::cout << "new character made of type: " << type << std::endl;
}

Character::~Character()
{
}



// loading stuff

void Character::load()
{
	Character::load("gamestate/characters/");
}

void Character::load(int levelnumber)
{
	Character::load("data/levels/" + std::to_string(levelnumber) + "/");
	// enemy, so flip sprite

	sprite.setScale(-1.0f, 1.0f);
}

void Character::load(std::string path)
{
	CharacterTemplate::load();

	std::string characterFilename;
	characterFilename = path + std::to_string(type) + ".character";

	std::fstream file(characterFilename, std::ios::in);
	if (!file.is_open()) {
		std::cout << "error loading file: " + path + std::to_string(type) + ".character" << std::endl;
	}

	Character::LoadInfo loadInfo;

	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}

	loadFileData(loadInfo);
	loadTextureData();
}

void Character::getFileLineData(int i, std::string & line, LoadInfo & loadInfo)
{
	switch (i)
	{
	case 1:
	{
		// TODO: make this safer, to avoid crashes
		loadInfo.level = std::stoi(line);
		break;
	}
	}
}

void Character::loadFileData(LoadInfo & loadInfo)
{
	levelFactor = (loadInfo.level * 1.0f) / 100.0f;
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

	loaded = true;
}








void Character::updatePos(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}





// animation and drawing

void Character::draw(sf::RenderWindow & window, sf::FloatRect boundBox, sf::Time elapsedTime)
{
	if (animating)
	{
		animationTime += elapsedTime;
		while (animationTime > timestep) {
			animationTime -= timestep;
			animationRect.left += animationRect.width;
			if (animationRect.left >= texture.getSize().x) {
				animating = false;
				break;
			}
			sprite.setTextureRect(animationRect);
		}
	}

	window.draw(sprite);
}


void Character::resetAnimation()
{
	animationRect.top = animationRect.height * 4;
	animationRect.left = 0;
	sprite.setTextureRect(animationRect);
	std::cout << "resetting animation for: " << name << std::endl;
}

void Character::startPrimaryAnimation(std::vector<Character>::iterator opponent)
{
	// TODO: make this do different animations, instead of just the one
	animationRect.top = getPrimaryMatchup(opponent) * animationRect.height;
	animationRect.left = 0;
	sprite.setTextureRect(animationRect);
	animating = true;

	std::cout << "in character, am: " << name << " fighting: " << opponent->name << std::endl;
}

void Character::startSecondaryAnimation(std::vector<Character>::iterator opponent)
{
	animationRect.top = animationRect.height*4;
	animationRect.left = 0;
	sprite.setTextureRect(animationRect);
	animating = true;
}





bool Character::isAnimationFinished()
{
	return !animating;
}






// battle mechanics

float Character::getPrimaryMatchup(std::vector<Character>::iterator opponent)
{
	return primaryMatchups[(opponent->primary_type - primary_type + 4)%4];
}

float Character::getSecondaryMatchup(std::vector<Character>::iterator opponent)
{
	return secondaryMatchups[(opponent->secondary_type - secondary_type + 5) % 5];
}

float Character::getAttack()
{
	return attack * levelFactor;
}

float Character::calculateDamage(std::vector<Character>::iterator opponent)
{
	std::cout << name << " attacking: " << opponent->name << ", matchup factor is: " << getPrimaryMatchup(opponent) << std::endl;
	return getAttack() * getPrimaryMatchup(opponent);
}
