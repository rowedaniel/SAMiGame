#pragma once

#include "../stdafx.h"
#include "Effects/Effect.h"

class Player
{
public:
	Player();
	Player(float maxHealth);
	~Player();
	
	void load(std::istream & file);



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

	struct LoadInfo
	{
	public:
		float maxHealth = 0.0f;
		float currentHealth = -1.0f;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);


	float maxHealth;
	float health;

	bool dead = false;

};