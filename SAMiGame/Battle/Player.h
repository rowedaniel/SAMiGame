#pragma once

#include "../stdafx.h"

class Player
{
public:
	Player();
	Player(float maxHealth);
	~Player();


	float getHealth();
	float getMaxHealth();
	void setHealth(float h);

	void damage(float damage);
	void addEffects();
	void calculateEffectDamage();

private:

	float maxHealth;
	float health;

	bool dead = false;

};