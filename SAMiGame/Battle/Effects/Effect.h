#pragma once

#include "../../stdafx.h"

class Character;
class Player; // to avoid include loops, instead include Player.h and Character.h in the cpp file.


class Effect
{

public:
	Effect(int d, float a, int sourceCharacterType, bool pOwned);
	~Effect();

	bool isExpired();
	void advanceTurn();

	// affects players:
	void (*beforeRound)(Effect & effect, Player * player);
	void (*afterRound)(Effect & effect, Player * player);
	float (*applyToDefense)(Effect & effect, float old, Player * player);

	// affects characters:
	void (*beforeAttack)(Effect & effect, Character * character);
	void (*afterAttack)(Effect & effect, Character * character);
	float (*applyToAttack)(Effect & effect, float old, Character * character);
	float (*applyToNewEffect)(Effect & effect, float old, Character * character);

	void setDescription(std::string s);
	std::string getDescription();
	float getAmount();

	bool isPlayerOwned();
	int getSourceCharacterType();


private:
	int duration;
	float amount;
	int sourceCharacterType;
	bool playerOwned;

	std::string description = "";

};