#include "Effect.h"
#include "../Character.h"
#include "../Player.h"


// applies to player
void doNothingPlayer(Effect & effect, Player * player) {};
float doNothingPlayerDefense(Effect & effect, float old, Player * player) { return old; };

void doNothingCharacter(Effect & effect, Character * character) {};
float doNothingCharacterAttack(Effect & effect, float old, Character * character) { return old; };


Effect::Effect(int d, float a, int characterType, bool pOwned)
{
	duration = d;
	amount = a;
	playerOwned = pOwned;
	sourceCharacterType = characterType;

	beforeRound = &doNothingPlayer;
	afterRound = &doNothingPlayer;
	applyToDefense = &doNothingPlayerDefense;

	// affects characters:
	beforeAttack = &doNothingCharacter;
	afterAttack = &doNothingCharacter;
	applyToAttack = &doNothingCharacterAttack;
	applyToNewEffect = &doNothingCharacterAttack;
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

void Effect::setDescription(std::string s)
{
	description = s;
}

std::string Effect::getDescription()
{
	return description;
}

float Effect::getAmount()
{
	return amount;
}

bool Effect::isPlayerOwned()
{
	return playerOwned;
}

int Effect::getSourceCharacterType()
{
	return sourceCharacterType;
}

