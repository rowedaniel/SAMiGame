#include "CharacterTemplate.h"

CharacterTemplate::CharacterTemplate(int characterType)
{
	type = characterType;
	std::cout << "new character template of type: " << characterType << std::endl;
}

CharacterTemplate::~CharacterTemplate()
{
}

void CharacterTemplate::load()
{
	std::fstream file("data/characters/"+std::to_string(type)+".characterTemplate", std::ios::in);
	if (!file.is_open()) {
		// Error
		// TODO: figure out error handling
		std::cout << "Error in opening file: " << "data/characters/" + std::to_string(type) + ".characterTemplate" << std::endl;
		return;
	}

	LoadInfo loadInfo;

	// load text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}

	// do stuff with the file data
	loadFileData(loadInfo);
	CharacterTemplate::loadTextureData();
}

void CharacterTemplate::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	switch (i) {
	case 1:
	{
		loadInfo.name = line;
		break;
	}
	case 2:
	{
		loadInfo.description = line;
		break;
	}
	case 3:
	{
		loadInfo.attack = std::stof(line);
		break;
	}
	case 4:
	{
		loadInfo.primary_type = std::stoi(line);
		break;
	}
	case 5:
	{
		loadInfo.secondary_type = std::stoi(line);
		break;
	}
	case 6:
	{
		loadInfo.secodary_effect_cooldown = std::stoi(line);
		break;
	}
	case 7:
	{
		loadInfo.numberOfCharacterEffects = std::stoi(line);
		loadInfo.totalNumberOfCharacterEffects = loadInfo.numberOfCharacterEffects;
		break;
	}
	case 8:
	{
		loadInfo.numberOfOpponentEffects = std::stoi(line);
		loadInfo.totalNumberOfOpponentEffects = loadInfo.numberOfOpponentEffects;
		break;
	}
	case 9:
	{
		loadInfo.numberOfPlayerEffects = std::stoi(line);
		loadInfo.totalNumberOfPlayerEffects = loadInfo.numberOfPlayerEffects;
		break;
	}
	case 10:
	{
		loadInfo.numberOfEnemyEffects = std::stoi(line);
		loadInfo.totalNumberOfEnemyEffects = loadInfo.numberOfEnemyEffects;
		break;
	}
	default:
	{
		i -= 9;

		// TODO: please, for the love of god, make these into a reasonable function

		// effects that influence this character
		if (loadInfo.numberOfCharacterEffects > 0) {
			if (loadInfo.extraLines == 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.characterEffectsText.push_back(std::string());
			}
			else {
				loadInfo.characterEffectsText.back().append(line + "\n");
				if (--loadInfo.extraLines == 0) {
					--loadInfo.numberOfCharacterEffects;
				}
			}
		}

		// effects that influence the opposing character
		else if (loadInfo.numberOfOpponentEffects > 0) {
			if (loadInfo.extraLines == 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.opponentEffectsText.push_back(std::string());
			}
			else {
				loadInfo.opponentEffectsText.back().append(line + "\n");
				if (--loadInfo.extraLines == 0) {
					--loadInfo.numberOfOpponentEffects;
				}
			}
		}

		// effects that influence this character's player
		else if (loadInfo.numberOfPlayerEffects > 0) {
			if (loadInfo.extraLines == 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.playerEffectsText.push_back(std::string());
			}
			else {
				loadInfo.playerEffectsText.back().append(line + "\n");
				if (--loadInfo.extraLines == 0) {
					--loadInfo.numberOfPlayerEffects;
				}
			}
		}


		// effects that influence the opposing character's player
		else if (loadInfo.numberOfEnemyEffects > 0) {
			if (loadInfo.extraLines == 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.enemyEffectsText.push_back(std::string());
			}
			else {
				loadInfo.enemyEffectsText.back().append(line + "\n");
				if (--loadInfo.extraLines == 0) {
					--loadInfo.numberOfEnemyEffects;
				}
			}
		}
		break;
	}
	}
}

void CharacterTemplate::loadFileData(LoadInfo & loadInfo)
{
	name = loadInfo.name;
	description = loadInfo.description;
	attack = loadInfo.attack;
	primary_type = loadInfo.primary_type;
	secondary_type = loadInfo.secondary_type;
	secondary_effect_cooldown = loadInfo.secodary_effect_cooldown;

	selfAppliedEffects.reserve(loadInfo.totalNumberOfCharacterEffects);
	opponentAppliedEffects.reserve(loadInfo.totalNumberOfOpponentEffects);
	selfPlayerAppliedEffects.reserve(loadInfo.totalNumberOfPlayerEffects);
	opponentPlayerAppliedEffects.reserve(loadInfo.totalNumberOfEnemyEffects);

	for (auto s : loadInfo.characterEffectsText) {
		selfAppliedEffects.push_back(EffectGetter::getEffectTemplate(s));
	}
	for (auto s : loadInfo.opponentEffectsText) {
		opponentAppliedEffects.push_back(EffectGetter::getEffectTemplate(s));
	}
	for (auto s : loadInfo.playerEffectsText) {
		selfPlayerAppliedEffects.push_back(EffectGetter::getEffectTemplate(s));
	}
	for (std::string s : loadInfo.enemyEffectsText) {
		opponentPlayerAppliedEffects.push_back(EffectGetter::getEffectTemplate(s));
	}


}

void CharacterTemplate::loadTextureData()
{
	if (!texture.loadFromFile("data/images/sprites/characters/" + std::to_string(type) + ".png")) {
		// error
		std::cout << "error loading character texture: data/images/sprites/characters/" + std::to_string(type) + ".png" << std::endl;
		return;
	}
}

