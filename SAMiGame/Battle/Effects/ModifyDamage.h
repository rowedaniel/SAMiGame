#pragma once
#include "../../stdafx.h"
#include "Effect.h"

class ModifyDamage : public Effect
{
public:
	float applyToDefense(float old, Player & player);
};