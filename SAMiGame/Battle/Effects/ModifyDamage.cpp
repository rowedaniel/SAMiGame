#include "ModifyDamage.h"

float ModifyDamage::applyToDefense(float old, Player & player)
{
	return amount * old;
}
