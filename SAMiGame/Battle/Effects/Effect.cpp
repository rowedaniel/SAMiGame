#include "Effect.h"

Effect::Effect(int d, float a)
{
	duration = d;
	amount = a;
}

Effect::~Effect()
{
}

bool Effect::isExpired()
{
	return duration <= 0;
}

void Effect::advanceTurn()
{
	--duration;
}


// applies to player

void Effect::beforeRound(Player & player)
{
}

void Effect::afterRound(Player & player)
{
}

float Effect::applyToDefense(float old, Player & player)
{
	return old;
}





// applies to character

void Effect::beforeAttack(std::vector<Character>::iterator character)
{
}

void Effect::afterAttack(std::vector<Character>::iterator character)
{
}

float Effect::applyToAttack(float old, std::vector<Character>::iterator character)
{
	return old;
}

float Effect::applyToNewSpecial(float old, std::vector<Character>::iterator character)
{
	return old;
}
