#pragma once
#include "../stdafx.h";
#include "CharacterTemplate.h";

class Character : public CharacterTemplate
{
public:
	Character(int characterType);
	~Character();

	void load(bool playerOwned);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	float getPrimaryMatchup(std::vector<Character>::iterator opponent);
	float getSecondaryMatchup(std::vector<Character>::iterator opponent);
private:
	const std::vector<float> primaryMatchups = {0.0f, 2.0f, 1.0f, 1.0f};
	const std::vector<float> secondaryMatchups = { 1.0f, 2.0f, 1.5f, 0.5f, 0.0f }; 

	void loadTextureData();

	sf::Sprite sprite;

};



/* 
// Old code to load character templates:

// TODO: move this to resource management system?
std::list<CharacterTemplate> loadedCharacterTemplates;
std::list<CharacterTemplate>::iterator getCharacterTemplate(int type);

// load Character Templates
std::set<int> added_character_templates;
// load templates for player character buttons
for (std::list<CharacterButton>::iterator it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
	if (added_character_templates.find(it->characterType) == added_character_templates.end()) {
		// new character, add it to template list
		loadedCharacterTemplates.push_back(CharacterTemplate(it->characterType));
		loadedCharacterTemplates.back().load();
		added_character_templates.insert(it->characterType);
	}
}
// load templates for enemy character buttons
for (std::list<CharacterButton>::iterator it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it) {
	if (added_character_templates.find(it->characterType) == added_character_templates.end()) {
		// new character, add it to template list
		loadedCharacterTemplates.push_back(CharacterTemplate(it->characterType));
		loadedCharacterTemplates.back().load();
		added_character_templates.insert(it->characterType);
	}
}



*/