#pragma once

#include "../stdafx.h"
#include "MenuItem.h"
#include "../Battle/Player.h"

class HealthBar : public MenuItem
{
public:
	HealthBar();
	~HealthBar();

	void load();
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox, sf::Time elapsedTime);

	void updatePos(sf::Vector2f pos);

	void setHealth(Player & player);
	bool isAnimationFinished();

private:

	sf::RectangleShape healthBox;


	float currentValue = 0.0f;
	float targetValue = 0.0f;
	float changeSpeed = 0.0f;

	void updateHealthDisplay(sf::Time elapsedTime);
};