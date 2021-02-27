#pragma once

#include "../stdafx.h"
#include "Effects/Effect.h"
#include "Effects/IncreaseDamage.h"
#include "Effects/ReduceDamage.h"
#include "Effects/IncreaseAttack.h"
#include "Effects/ReduceAttack.h"
#include "Effects/FlatDamage.h"
#include "Effects/IncreaseEffectCooldown.h"

class EffectGetter
{
public:

	struct EffectInfo
	{
		int type = 0;
		int duration = 0;
		float amount = 0;
	};

	static Effect getEffect(EffectInfo info, int sourceCharacterType, bool pOwned);
	static EffectInfo getEffectTemplate(std::string text);
};