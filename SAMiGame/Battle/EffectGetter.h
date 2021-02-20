#pragma once

#include "../stdafx.h"
#include "Effects/Effect.h"

class EffectGetter
{
public:
	EffectGetter();
	~EffectGetter();

	Effect getEffect(int type, int duration, float amount);
};