#pragma once
#include "stdafx.h"



class VisibleGameObject
{
public:
	VisibleGameObject();
	virtual ~VisibleGameObject();

	virtual void load(std::string filename);
	virtual void draw(sf::RenderWindow & window);

	virtual void setPosition(float x, float y);

private:
	sf::Texture _texture;
	sf::Sprite _sprite;
};