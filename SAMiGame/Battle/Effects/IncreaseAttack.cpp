#include "IncreaseAttack.h"
#include "../Character.h"



float increaseAttack(Effect & effect, float old, Character * character) {
	std::cout << "AAAAAAAAAAAAAAAA increasing attack with amout: " << effect.getAmount() << std::endl;
	return (effect.getAmount() + 1.0f) * old;
}

Effect IncreaseAttack(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.applyToAttack = &increaseAttack;
	e.setDescription("Increase attack damage by " + std::to_string((int)(std::round(a * 100.0f))) + "%");
	return e;
}
