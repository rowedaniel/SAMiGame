#include "HealthBar.h"

HealthBar::HealthBar()
{

}

HealthBar::~HealthBar()
{
}

void HealthBar::load()
{
	std::fstream file("data/healthBar.menuItem", std::ios::in);
	if (file.is_open()) {
		MenuItem::load(file);
	}
	healthBox.setFillColor(sf::Color(190, 50, 90));
	healthBox.setSize(sf::Vector2f(boundingBox.width - borderSpriteSize, boundingBox.height - borderSpriteSize));
}

void HealthBar::draw(sf::RenderWindow & window, sf::FloatRect boundBox , sf::Time elapsedTime)
{
	drawBackground(window, boundBox);

	updateHealthDisplay(elapsedTime);
	drawSubitem(window, healthBox, boundBox);
	// draw active effects at bottom
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->draw(window, boundBox);
	}
	
	drawBorder(window, boundBox);
}

void HealthBar::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();
}

void HealthBar::updateItemPos()
{
	sf::Vector2f pos = sf::Vector2f(boundingBox.left, boundingBox.top);
	healthBox.setPosition(pos + sf::Vector2f(borderSpriteSize / 2, boundingBox.height - borderSpriteSize / 2));

	float y = pos.y + boundingBox.height + 16.0f;
	const float x = pos.x - 2.0f - (boundingBox.width - 4.0f) * !playerOwned;
	const float yIncrement =  32.0f;
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->updatePos(sf::Vector2f(x, y));
		y += yIncrement;
		std::cout << "in healthbar::updateItemPos, putting effectbutton at: " << x << ", " << y << std::endl;
	}
}








void HealthBar::update(Player & player, bool isPlayer)
{
	playerOwned = isPlayer;
	targetValue = std::max(player.getHealth() / player.getMaxHealth(), 0.0f);
	// it should take 1 second to finish health movement
	changeSpeed = (targetValue - currentValue) / 0.5f;

	std::cout << "starting healthbar animation, going from: " << currentValue << " to: " << targetValue << " at speed: " << changeSpeed << std::endl;


	activeEffectButtons.clear();
	for (auto it = player.activeEffects.begin(); it != player.activeEffects.end(); ++it) {
		activeEffectButtons.push_back(EffectButton());
		auto info = --activeEffectButtons.end();
		info->load();
		info->setEffectData(it);
	}
	updateItemPos();
}

bool HealthBar::isAnimationFinished()
{
	return currentValue == targetValue;
}


void HealthBar::updateHealthDisplay(sf::Time elapsedTime)
{
	float expectedNextStep = changeSpeed * elapsedTime.asSeconds();
	if (abs(targetValue - currentValue) < abs(expectedNextStep)) {
		currentValue = targetValue;
	}
	else {

		currentValue += expectedNextStep;
		//std::cout << "updating healthBar pos, currently at: " << currentValue << std::endl;
	}

	healthBox.setScale(1.0f, -currentValue);
}

void HealthBar::checkMouseMove(sf::Vector2f pos)
{
	for (auto it = activeEffectButtons.begin(); it != activeEffectButtons.end(); ++it) {
		it->checkMouseMove(pos);
	}
}
