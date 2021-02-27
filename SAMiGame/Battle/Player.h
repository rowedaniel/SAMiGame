#pragma once

#include "../stdafx.h"
#include "Effects/Effect.h"

class Player
{
public:
	Player();
	Player(float maxHealth);
	~Player();


	float getHealth();
	float getMaxHealth();
	void setHealth(float h);

	void flatDamage(float damage);
	void damage(float damage);
	void addEffect(Effect effect);

	bool isDead();

	std::list<Effect> activeEffects;
	
	void beforeRoundEffects();
	void afterRoundEffects();
	
	void incrementEffectTimer();

private:

	float maxHealth;
	float health;

	bool dead = false;

};