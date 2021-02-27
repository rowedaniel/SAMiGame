#include "EffectGetter.h"


Effect EffectGetter::getEffect(EffectInfo info, int sourceCharacterType, bool pOwned)
{
	switch (info.type)
	{
	case 1:
	{
		std::cout << "creating new IncraseDamage effect!" << std::endl;
		return IncreaseDamage(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 2:
	{
		std::cout << "creating new ReduceDamage effect!" << std::endl;
		return ReduceDamage(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 3:
	{
		std::cout << "creating new IncreaseAttack effect!" << std::endl;
		return IncreaseAttack(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 4:
	{
		std::cout << "creating new ReduceAttack effect!" << std::endl;
		return ReduceAttack(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 5:
	{
		std::cout << "creating new IncreaseEffectCooldown effect!" << std::endl;
		return IncreaseEffectCooldown(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 6:
	{
		std::cout << "creating new ReduceEffectCooldown effect! (not implemented)" << std::endl;
		// TODO: implement this
		return IncreaseEffectCooldown(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	case 7:
	{
		std::cout << "creating new FlatDamage effect!" << std::endl;
		return FlatDamage(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	default:
	{
		return Effect(info.duration, info.amount, sourceCharacterType, pOwned);
	}
	}
}

EffectGetter::EffectInfo EffectGetter::getEffectTemplate(std::string text)
{
	EffectInfo outputInfo;
	
	std::istringstream textStream(text);
	std::string line;
	int i = 0;
	while (getline(textStream, line)) {
		switch (++i)
		{
		case 1:
		{
			outputInfo.type = std::stoi(line);
			break;
		}
		case 2:
		{
			outputInfo.duration = std::stoi(line);
			break;
		}
		case 3:
		{
			outputInfo.amount = std::stof(line);
			break;
		}
		default:
			break;
		}
	}


	return outputInfo;
}
