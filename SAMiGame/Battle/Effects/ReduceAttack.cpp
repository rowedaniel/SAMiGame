#include "ReduceAttack.h"
#include "../Character.h"



float reduceAttack(Effect & effect, float old, Character * character) {
	return (1.0f - effect.getAmount()) * old;
}

Effect ReduceAttack(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.applyToAttack = &reduceAttack;
	e.setDescription("Reduce attack damage by " + std::to_string((int)(std::round(a * 100.0f))) + "%");
	return e;
}
