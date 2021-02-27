#pragma once

#include "../stdafx.h"
#include "MenuItem.h"
#include "../Battle/Player.h"
#include "EffectButton.h"

class HealthBar : public MenuItem
{
public:
	HealthBar();
	~HealthBar();

	void load();
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox, sf::Time elapsedTime);

	void updatePos(sf::Vector2f pos);

	void update(Player & player, bool isPlayer);
	bool isAnimationFinished();

	// for displaying active effects
	void checkMouseMove(sf::Vector2f pos);

private:

	bool playerOwned = false;

	sf::RectangleShape healthBox;


	float currentValue = 0.0f;
	float targetValue = 0.0f;
	float changeSpeed = 0.0f;

	void updateItemPos();
	void updateHealthDisplay(sf::Time elapsedTime);

	std::list<EffectButton> activeEffectButtons;
};