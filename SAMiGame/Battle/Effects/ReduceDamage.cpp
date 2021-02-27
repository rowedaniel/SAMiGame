#include "ReduceDamage.h"

float reduceDamage(Effect & effect, float old, Player * player)
{
	return (1.0f - effect.getAmount()) * old;
}

Effect ReduceDamage(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.applyToDefense = &reduceDamage;
	e.setDescription("Reduce damage taken by " + std::to_string((int)(std::round(a * 100.0f))) + "%");
	return e;
}