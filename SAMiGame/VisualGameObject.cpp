#include "VisualGameObject.h"
#include "stdafx.h"

VisibleGameObject::VisibleGameObject()
{
}

VisibleGameObject::~VisibleGameObject()
{
	// TODO: figure out how deconstructors work
	/*delete & _texture;
	delete & _sprite;*/
}

void VisibleGameObject::load(std::string filename)
{
	if (_texture.loadFromFile(filename) != true)
	{
		// error
		std::cout << "Error Loading Texture: " << filename << std::endl;
		return;
	}

	std::cout << "Loading: " << filename << std::endl;

	_sprite.setTexture(_texture);

	_sprite.setPosition(100, 25);
}

void VisibleGameObject::draw(sf::RenderWindow & window)
{
	window.draw(_sprite);
}

void VisibleGameObject::setPosition(float x, float y)
{
	_sprite.setPosition(x, y);
}