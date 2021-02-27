#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "MenuText.h"
#include "../Battle/Effects/Effect.h"

class EffectButton : public MenuItem
{
public:
	EffectButton();
	~EffectButton();

	void load();
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox);

	bool contains(sf::Vector2f pos);

	void updatePos(sf::Vector2f pos);

	void setEffectData(std::list<Effect>::iterator sourceEffect);
	std::string getDescription();


	void checkMouseMove(sf::Vector2f pos);

	std::list<Effect>::iterator effectRef;

private:

	void loadTextureData();
	void updateItemPos();

	sf::Texture bgTexture; // TODO: move all textures to a resource manager of some sort
	sf::Sprite bgSprite;

	sf::Texture characterTexture;
	sf::Sprite characterSprite;

	MenuText descriptionText;
	MenuItem descriptionBg;

	std::string description;

	float descriptionOffset = 16.0f;
	bool showDescription = false;
};