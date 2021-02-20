#include "EffectGetter.h"

EffectGetter::EffectGetter()
{
}

EffectGetter::~EffectGetter()
{
}

Effect EffectGetter::getEffect(int type, int duration, float amount)
{
	switch (type)
	{
	default:
	{
		return Effect(duration, amount);
	}
	}
}