#include "../stdafx.h"
#include "MenuButton.h"

MenuButton::MenuButton()
{
}

MenuButton::~MenuButton()
{
}


void MenuButton::load(std::istream& file)
{
	// TODO: figure out how to integrate this with superclass
	MenuButton::LoadInfo loadInfo;

	// load text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		std::cout << line << std::endl;
		getFileLineData(++i, line, loadInfo);
	}

	// do stuff with the file data
	loadFileData(loadInfo);

	// now, load everything else
	loadTextureData();

	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
}

void MenuButton::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	MenuItem::draw(window, boundBox);
	drawSubitem(window, text, boundBox);
}

void MenuButton::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);

	sf::FloatRect textRect = text.getGlobalBounds();
	text.setPosition(pos + sf::Vector2f((boundingBox.width - textRect.width) / 2.0f, (boundingBox.height - textRect.height * 2.0) / 2.0f));
}

void MenuButton::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
	if (i < 6) { // don't bother for early stuff
		return;
	}
	switch (i) {
	case 6:
	{
		loadInfo.fontSize = std::stoi(line);
		break;
	}
	case 7:
	{
		loadInfo.text = line;
		break;
	}
	}
}

void MenuButton::loadFileData(LoadInfo & loadInfo)
{
	text.setCharacterSize(loadInfo.fontSize);
	text.setString(loadInfo.text);
	MenuItem::loadFileData(loadInfo.oldInfo);
}

void MenuButton::loadTextureData()
{
	MenuItem::loadTextureData();
	if (!font.loadFromFile("fonts/arial-cufonfonts/ARIAL.ttf")) {
		// error handling
	}
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
}

void MenuButton::drawSubitem(sf::RenderWindow & window, sf::Text & item, sf::FloatRect & boundBox)
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