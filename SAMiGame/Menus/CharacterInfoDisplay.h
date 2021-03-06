#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "MenuText.h"
#include "../Battle/Character.h"
#include "../Battle/EffectGetter.h"
#include "EffectButton.h"

// TODO: make parent 'clickable' class. Also, consider adding more parent classes.
class CharacterInfoDisplay : public MenuItem
{
public:
	CharacterInfoDisplay();
	~CharacterInfoDisplay();

	void setCharacterData(std::vector<Character>::iterator sourceCharacter);
	void load(std::istream & file);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	void updatePos(sf::Vector2f pos);
	void updateItemPos();


	void checkMouseMove(sf::Vector2f pos);

private:
	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		std::string characterNameText;
		std::string characterOtherText;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	Effect getEffectInfo(std::vector<EffectGetter::EffectInfo>::iterator effect, int characterType, bool playerOwned);
	void loadEffectButton(std::list<Effect> effectList, std::list<EffectButton>& effectButtonList);

	MenuText characterName;
	//MenuText characterDescription;
	MenuText characterAttack;
	
	sf::Texture typeTexture; // TODO: move all textures to a resource manager of some sort
	sf::Sprite primaryTypeSprite;
	sf::Sprite secondaryTypeSprite;

	// effects which this character causes
	std::list<EffectButton> causedEffects;

	// effects active on this character
	std::list<EffectButton> activeEffects;
};
