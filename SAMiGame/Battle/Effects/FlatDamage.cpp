#include "FlatDamage.h"
#include "../Player.h"



void flatDamage(Effect & effect, Player * player) {
	player->flatDamage(effect.getAmount());
}

Effect FlatDamage(int d, float a, int sourceCharacterType, bool pOwned)
{
	Effect e = Effect(d, a, sourceCharacterType, pOwned);
	e.afterRound = &flatDamage;
	e.setDescription("Deal " + std::to_string((int)a) + " flat damage");
	return e;
}
