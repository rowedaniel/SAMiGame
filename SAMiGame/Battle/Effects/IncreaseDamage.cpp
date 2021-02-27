#include "IncreaseDamage.h"

float increaseDamage(Effect & effect, float old, Player * player)
{
	return (effect.getAmount() + 1.0f) * old;
}

Effect IncreaseDamage(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.applyToDefense = &increaseDamage;
	e.setDescription("Increase damage taken by " + std::to_string((int)(std::round(a * 100.0f))) + "%");
	return e;
}
