#pragma once
#include "../stdafx.h"
#include "EffectGetter.h"

class CharacterTemplate
{
public:
	CharacterTemplate(int characterType);
	~CharacterTemplate();

	int type; // which character, kevin, etc.
	std::string name;
	std::string description;
	int primary_type; // the actual type. TODO: make this terminology less confusing
	int secondary_type;

	void load();

protected:
	struct LoadInfo
	{
		std::string name;
		std::string description;
		float attack = 0;
		unsigned int primary_type = 0;
		unsigned int secondary_type = 0;
		unsigned int secodary_effect_cooldown = 1;

		unsigned int extraLines = 0;

		unsigned int numberOfCharacterEffects = 0;
		unsigned int totalNumberOfCharacterEffects = 0;

		unsigned int numberOfOpponentEffects = 0;
		unsigned int totalNumberOfOpponentEffects = 0;

		unsigned int numberOfPlayerEffects = 0;
		unsigned int totalNumberOfPlayerEffects = 0;

		unsigned int numberOfEnemyEffects = 0;
		unsigned int totalNumberOfEnemyEffects = 0;


		std::list<std::string> characterEffectsText;
		std::list<std::string> opponentEffectsText;
		std::list<std::string> playerEffectsText;
		std::list<std::string> enemyEffectsText;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	sf::Texture texture;


	float attack;
	int secondary_effect_cooldown;


	std::vector<EffectGetter::EffectInfo> selfAppliedEffects;
	std::vector<EffectGetter::EffectInfo> opponentAppliedEffects;
	std::vector<EffectGetter::EffectInfo> selfPlayerAppliedEffects;
	std::vector<EffectGetter::EffectInfo> opponentPlayerAppliedEffects;
};