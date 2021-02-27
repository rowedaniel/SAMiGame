#include "IncreaseEffectCooldown.h"
#include "../Character.h"



void increaseEffectCooldown(Effect & effect, Character * character) {
	std::cout << "AAAAAAAAAAAAAAAAAAA in IncreaseEffectCooldown, amount is: " << effect.getAmount() << std::endl;
	character->increaseEffectCooldown((int)effect.getAmount());
}

Effect IncreaseEffectCooldown(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.afterAttack = &increaseEffectCooldown;
	e.setDescription("Increase effect cooldown by " + std::to_string((int)a) + " turns");
	return e;
}
