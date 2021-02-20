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
	
	drawBorder(window, boundBox);
}

void HealthBar::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	healthBox.setPosition(pos + sf::Vector2f(borderSpriteSize / 2, boundingBox.height - borderSpriteSize / 2));
}

void HealthBar::setHealth(Player & player)
{
	targetValue = player.getHealth() / player.getMaxHealth();
	// it should take 1 second to finish health movement
	changeSpeed = (targetValue - currentValue) / 0.5f;

	std::cout << "starting healthbar animation, going from: " << currentValue << " to: " << targetValue << " at speed: " << changeSpeed << std::endl;
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
