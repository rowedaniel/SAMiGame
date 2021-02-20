#pragma once

#include "../../stdafx.h"
#include "../Character.h"
#include "../Player.h"

class Effect
{

public:
	Effect(int d, float a);
	~Effect();

	bool isExpired();
	void advanceTurn();

	// affects players:
	void beforeRound(Player & player);
	void afterRound(Player & player);
	float applyToDefense(float old, Player & player);

	// affects characters:
	void beforeAttack(std::vector<Character>::iterator character);
	void afterAttack(std::vector<Character>::iterator character);
	float applyToAttack(float old, std::vector<Character>::iterator character);
	float applyToNewSpecial(float old, std::vector<Character>::iterator character);



protected:
	int type;
	int duration;
	float amount;

};