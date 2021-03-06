#include "../stdafx.h"
#include "MenuItem.h"
#include <string>
#include <sstream>

MenuItem::MenuItem()
{
}

MenuItem::~MenuItem()
{
}


void MenuItem::load(std::istream& file)
{
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

	// now, load everything else
	loadTextureData();

	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
}



void MenuItem::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	drawBackground(window, boundBox);
	drawBorder(window, boundBox);
}

void MenuItem::drawBackground(sf::RenderWindow & window, sf::FloatRect boundBox)
{

	// draw menu background
	drawSubitem(window, background, boundBox);
}

void MenuItem::drawBorder(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	// draw menu borders (these should go on top of contents, in case of overflows
	drawSubitem(window, tlCorner, boundBox);
	drawSubitem(window, blCorner, boundBox);
	drawSubitem(window, trCorner, boundBox);
	drawSubitem(window, brCorner, boundBox);
	drawSubitem(window, tEdge, boundBox);
	drawSubitem(window, bEdge, boundBox);
	drawSubitem(window, lEdge, boundBox);
	drawSubitem(window, rEdge, boundBox);
}





int MenuItem::getWidth()
{
	return (int)boundingBox.width;
}

int MenuItem::getHeight()
{
	return (int)boundingBox.height;
}

void MenuItem::isLoaded()
{
}

void MenuItem::updatePos(sf::Vector2f pos)
{
	boundingBox.left = pos.x;
	boundingBox.top = pos.y;

	// set subitem positions
	background.setPosition(pos + sf::Vector2f(borderSpriteSize / 2, borderSpriteSize / 2));
	tlCorner.setPosition(pos);
	blCorner.setPosition(pos + sf::Vector2f(0, boundingBox.height));
	trCorner.setPosition(pos + sf::Vector2f(boundingBox.width, 0));
	brCorner.setPosition(pos + sf::Vector2f(boundingBox.width, boundingBox.height));
	tEdge.setPosition(pos + sf::Vector2f(borderSpriteSize, 0));
	lEdge.setPosition(pos + sf::Vector2f(0, boundingBox.height - borderSpriteSize));
	rEdge.setPosition(pos + sf::Vector2f(boundingBox.width, borderSpriteSize));
	bEdge.setPosition(pos + sf::Vector2f(boundingBox.width - borderSpriteSize, boundingBox.height));

}






void MenuItem::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	switch (i) {
	case 1:
	{
		loadInfo.width = std::stoi(line);
		break;
	}
	case 2:
	{
		loadInfo.height = std::stoi(line);
		break;
	}
	case 3:
	{
		loadInfo.red = std::stoi(line);
		break;
	}
	case 4:
	{
		loadInfo.green = std::stoi(line);
		break;
	}
	case 5:
	{
		loadInfo.blue = std::stoi(line);
		break;
	}
	}
}

void MenuItem::loadFileData(LoadInfo & loadInfo)
{

	// now that we have the data, actually set the values
	// menu size
	boundingBox.width = (float)loadInfo.width;
	boundingBox.height = (float)loadInfo.height;
	background.setSize(sf::Vector2f(boundingBox.width - borderSpriteSize, boundingBox.height - borderSpriteSize));

	// menu color
	sf::Color color = sf::Color(loadInfo.red, loadInfo.green, loadInfo.blue);
	background.setFillColor(color);
	tlCorner.setColor(color);
	blCorner.setColor(color);
	trCorner.setColor(color);
	brCorner.setColor(color);
	tEdge.setColor(color);
	bEdge.setColor(color);
	lEdge.setColor(color);
	rEdge.setColor(color);
}

void MenuItem::loadTextureData()
{
	// load all the menu border images
	// TODO: put parts of this into parent class for both menu buttons and menus
	if (!texture.loadFromFile("data/images/sprites/button2.png")) {
		// error
		return;
	}
	texture.setSmooth(false);
	texture.setRepeated(true);
	// load corners by rotating the first part of the image
	tlCorner = sf::Sprite(texture, sf::IntRect(0, 0, borderSpriteSize, borderSpriteSize));
	blCorner = sf::Sprite(texture, sf::IntRect(0, 0, borderSpriteSize, borderSpriteSize));
	blCorner.setRotation(270.0f);
	trCorner = sf::Sprite(texture, sf::IntRect(0, 0, borderSpriteSize, borderSpriteSize));
	trCorner.setRotation(90.0f);
	brCorner = sf::Sprite(texture, sf::IntRect(0, 0, borderSpriteSize, borderSpriteSize));
	brCorner.setRotation(180.0f);

	// load edges by rotating/scaling the second part of the image
	tEdge = sf::Sprite(texture, sf::IntRect(0, borderSpriteSize, ((int)boundingBox.width - borderSpriteSize * 2), borderSpriteSize));
	bEdge = sf::Sprite(texture, sf::IntRect(0, borderSpriteSize, ((int)boundingBox.width - borderSpriteSize * 2), borderSpriteSize));
	bEdge.setRotation(180);
	lEdge = sf::Sprite(texture, sf::IntRect(0, borderSpriteSize, ((int)boundingBox.height - borderSpriteSize * 2), borderSpriteSize));
	lEdge.setRotation(270);
	rEdge = sf::Sprite(texture, sf::IntRect(0, borderSpriteSize, ((int)boundingBox.height - borderSpriteSize * 2), borderSpriteSize));
	rEdge.setRotation(90);

}




void MenuItem::drawSubitem(sf::RenderWindow & window, sf::Sprite & item, sf::FloatRect & boundBox)
{
	sf::FloatRect intersection;
	sf::FloatRect itemBounds = item.getGlobalBounds();

	// only draw if on screen
	if (boundBox.intersects(itemBounds, intersection)) {
		if (intersection.width != itemBounds.width || intersection.height != itemBounds.height) {
			// part of the subitem is off of the menu, so scale it down to keep it in.
			if (intersection.width < itemBounds.width) {
				item.setScale((intersection.width - borderSpriteSize / 2.0f) / itemBounds.width, 1.0f);
			}
			if (intersection.height < itemBounds.height) {
				item.setScale((intersection.height - borderSpriteSize / 2.0f) / itemBounds.height, 1.0f);
			}

			// sometimes, the scaling makes the subitem go out of position, and this moves it back.
			itemBounds = item.getGlobalBounds();
			//std::cout << "intersection left: " << intersection.left << " itemBounds left: " << itemBounds.left << std::endl;
			item.move(sf::Vector2f(intersection.left - itemBounds.left + (intersection.left == boundBox.left ? borderSpriteSize / 2.0 : 0),
				intersection.top - itemBounds.top + (intersection.top == boundBox.top ? borderSpriteSize / 2.0 : 0)
			));
		}
		else
		window.draw(item);
	}
}

// TODO: figure out a way to avoid repeating this.
// NOTE: there's another version of this in MenuButton.
void MenuItem::drawSubitem(sf::RenderWindow & window, sf::RectangleShape & item, sf::FloatRect & boundBox)
{
	sf::FloatRect intersection;
	sf::FloatRect itemBounds = item.getGlobalBounds();

	if (boundBox.intersects(itemBounds, intersection)) {
		if (intersection.width != itemBounds.width || intersection.height != itemBounds.height) {
			// part of the subitem is off of the menu, so scale it down to keep it in.
			if (intersection.width < itemBounds.width) {
				item.setScale((intersection.width - borderSpriteSize / 2.0f) / itemBounds.width, 1.0f);
			}
			if (intersection.height < itemBounds.height) {
				item.setScale(1.0f, (intersection.height - borderSpriteSize / 2.0f) / itemBounds.height);
			}
			// sometimes, the scaling makes the subitem go out of position, and this moves it back.
			itemBounds = item.getGlobalBounds();
			item.move(sf::Vector2f(intersection.left - itemBounds.left + (intersection.left == boundBox.left ? borderSpriteSize / 2.0 : 0),
				intersection.top - itemBounds.top + (intersection.top == boundBox.top ? borderSpriteSize / 2.0 : 0)
			));
		}
		window.draw(item);
	}
}
