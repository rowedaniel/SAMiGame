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
	playerOwned = true;
	Character::load("gamestate/characters/");
}

void Character::load(int levelnumber)
{
	playerOwned = false;
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
	case 2:
	{
		loadInfo.xp = std::stof(line);
		break;
	}
	}
}

void Character::loadFileData(LoadInfo & loadInfo)
{
	level = loadInfo.level;
	xp = loadInfo.xp;
	levelFactor = (loadInfo.level * 1.0f) / 100.0f;
	cooldownTimer = secondary_effect_cooldown;
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
	resetAnimation();

	loaded = true;
}









void Character::updatePos(sf::Vector2f pos)
{
	position = pos;
	updateSpritePos();
	updateItemPos();
}



void Character::updateSpritePos()
{
	sprite.setPosition(position + sf::Vector2f(animationOffset, 0.0f));
}

void Character::updateItemPos()
{
	float x = (playerOwned - 1.0f) * 64.0f + 16.0f;
	const float y = 96.0f;
	const float xIncrement = 32.0f * (playerOwned * 2.0f - 1.0f);
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->updatePos(position + sf::Vector2f(x, y));
		x += xIncrement;
	}
}





sf::FloatRect Character::getBounds()
{
	return sprite.getGlobalBounds();
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
			// TODO: check if this is good practice
			if ((unsigned int)animationRect.left >= texture.getSize().x) {
				animating = false;
				break;
			}
			sprite.setTextureRect(animationRect);
		}
		// TODO: figure out how to deal with animation movement
		// TODO: testing only, remove later
		//animationOffset += 550.0f * elapsedTime.asSeconds() * (playerOwned * 2.0f - 1.0f);
		updateSpritePos();
	}

	window.draw(sprite);

	// draw active effects at bottom
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->draw(window, boundBox);
	}
}


void Character::resetAnimation()
{
	animationOffset = 0.0f;
	animationRect.top = animationRect.height * 3;
	animationRect.left = 0;
	sprite.setTextureRect(animationRect);
	std::cout << "resetting animation for: " << name << std::endl;
}

void Character::startPrimaryAnimation(std::vector<Character>::iterator opponent)
{
	animationOffset = 0.0f;
	animationRect.top = (int)(getPrimaryMatchup(opponent) * animationRect.height);
	animationRect.left = 0;
	sprite.setTextureRect(animationRect);
	animating = true;

	std::cout << "in character, am: " << name << " fighting: " << opponent->name << std::endl;
}

void Character::startSecondaryAnimation(std::vector<Character>::iterator opponent)
{
	animationOffset = 0.0f;
	animationRect.top = animationRect.height*3;
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
	float attack = getAttack() * getPrimaryMatchup(opponent);
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		std::cout << "In Character, damage before: " << attack << std::endl;
		attack = (*it->applyToAttack)(*it, attack, this);
		std::cout << "In Character, damage after: " << attack << std::endl;
	}

	return attack;
}

int Character::getEffectCooldown()
{
	return cooldownTimer;
}

void Character::increaseEffectCooldown(int duration)
{
	cooldownTimer = std::max(0, cooldownTimer + duration);
}

EffectGetter::EffectInfo Character::calculateSecondaryAmount(std::vector<Character>::iterator opponent, EffectGetter::EffectInfo & inputInfo)
{
	EffectGetter::EffectInfo outputInfo;

	outputInfo.type = inputInfo.type;
	outputInfo.duration = inputInfo.duration;
	outputInfo.amount = inputInfo.amount * getSecondaryMatchup(opponent);
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		std::cout << "In Character, effect amount before: " << outputInfo.amount << std::endl;
		outputInfo.amount = (*it->applyToAttack)(*it, outputInfo.amount, this);
		std::cout << "In Character, effect amount after: " << outputInfo.amount << std::endl;
	}

	return outputInfo;
	
}

void Character::applySecondary(std::vector<Character>::iterator opponent, Player & player, Player & opponentPlayer)
{
	if (cooldownTimer > 0 || getSecondaryMatchup(opponent) == 0) {
		return;
	}
	cooldownTimer = secondary_effect_cooldown;

	std::cout << "applying secondary effects!" << std::endl;
	for (auto it : selfAppliedEffects)
	{
		addEffect(EffectGetter::getEffect(calculateSecondaryAmount(opponent, it), type, playerOwned));
	}
	for (auto it : opponentAppliedEffects)
	{
		opponent->addEffect(EffectGetter::getEffect(calculateSecondaryAmount(opponent, it), type, playerOwned));
	}
	for (auto it : selfPlayerAppliedEffects)
	{
		player.addEffect(EffectGetter::getEffect(calculateSecondaryAmount(opponent, it), type, playerOwned));
	}
	for (auto it : opponentPlayerAppliedEffects)
	{
		opponentPlayer.addEffect(EffectGetter::getEffect(calculateSecondaryAmount(opponent, it), type, playerOwned));
	}
}

void Character::addEffect(Effect effect)
{
	activeEffects.push_back(effect);
	activeEffectButtons.push_back(EffectButton());
	auto it = --activeEffectButtons.end();
	it->load();
	it->setEffectData(--activeEffects.end());

	updateItemPos();
}

void Character::incrementEffectTimer()
{
	auto it = activeEffects.begin();
	auto itButton = activeEffectButtons.begin();
	while(it != activeEffects.end()) {
		it->advanceTurn();
		if (it->isExpired())
		{
			it = activeEffects.erase(it);
			itButton = activeEffectButtons.erase(itButton);
			continue;
		}
		++it;
		++itButton;
	}

	if (cooldownTimer > 0) {
		--cooldownTimer;
	}
}

void Character::beforeAttackEffects()
{
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		(*it->beforeAttack)(*it, this);
	}
}

void Character::afterAttackEffects()
{
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		(*it->beforeAttack)(*it, this);
	}
}

std::list<Effect> Character::getEffects()
{
	return activeEffects;
}

bool Character::isPlayerOwned()
{
	return playerOwned;
}

void Character::checkMouseMove(sf::Vector2f pos)
{
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->checkMouseMove(pos);
	}
}










void Character::updateXp(std::vector<Character>& opponents)
{
	for (auto it = opponents.begin(); it != opponents.end(); ++it) {
		xp += (it->getLevel() * 1.0f) / (level * 1.0f) / 3.0f / std::pow(1.01f, level);
		if (xp > 1.0f) {
			xp = 0;
			level += 1;
		}
	}

	// assume player character, because enemies don't level up
	std::fstream unlockedButtonFile("gamestate/characters/" + std::to_string(type) + ".character", std::ios::out);
	if (unlockedButtonFile.is_open()) {
		unlockedButtonFile << std::to_string(level) << std::endl;
		unlockedButtonFile << std::to_string(xp) << std::endl;
	}
	else {
		std::cout << "error opening file: gamestate/characters/" + std::to_string(type) + ".character";
	}
}

int Character::getLevel()
{
	return level;
}
