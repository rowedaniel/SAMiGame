#include "../stdafx.h"
#include "MenuText.h"

MenuText::MenuText()
{
}

MenuText::~MenuText()
{
}


void MenuText::load(std::istream& file)
{
	MenuText::LoadInfo loadInfo;

	// load text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}


	// unlike every other time, I need to load font data first, so that the sizing is right later on.
	// now, load everything else
	loadTextureData();

	// do stuff with the file data
	loadFileData(loadInfo);


	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
}

void MenuText::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	//// TODO: testing only, remove this later
	//drawBackground(window, boundBox);

	// text doesn't get borders
	//MenuItem::draw(window, boundBox);
	drawSubitem(window, text, boundBox);
}

void MenuText::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateTextPos();
}

void MenuText::setText(std::string txt)
{
	setText(txt, 0, 10);
}

void MenuText::setText(std::string txt, int usedchrs, int n)
{
	text.setString(txt);
	updateTextPos();

	if (n <= 0) {
		return;
	}

	const sf::FloatRect textSize = text.getGlobalBounds();
	if (textSize.width > boundingBox.width) {
		//std::cout << "too big, " << txt << " size: " << textSize.width << " out of " << boundingBox.width << std::endl;
		int pos = (int)(boundingBox.width) * 2 / text.getCharacterSize() + usedchrs * 1;
		//std::cout << "starting at: " << pos << ", walking back to find nearest space" << std::endl;
		pos = txt.find_last_of(" ", pos);
		//std::cout << "replacing: " << pos << " of: " << sizeof txt << std::endl;

		if (pos < txt.length()) {
			txt.replace(pos, 1, "\n");
			//std::cout << "recursing, n is: " << n << std::endl;
			setText(txt, pos, n - 1);
		}
		else {
			//std::cout << "unable to find nearest space" << std::endl;
		}
	}
	else {
		//std::cout << "right size, " << txt << " length: " << textSize.width << " out of " << boundingBox.width << std::endl;
	}
}




void MenuText::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
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

void MenuText::loadFileData(LoadInfo & loadInfo)
{

	MenuItem::loadFileData(loadInfo.oldInfo);

	text.setCharacterSize(loadInfo.fontSize);
	setText(loadInfo.text);


	//// TODO: remove this later, testing only
	//sf::Color color = sf::Color(100, 200, 50);
	//background.setFillColor(color);
}

void MenuText::loadTextureData()
{
	MenuItem::loadTextureData();
	if (!font.loadFromFile("data/fonts/arial-cufonfonts/ARIAL.ttf")) {
		// error handling
	}
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
}

void MenuText::updateTextPos()
{

	sf::FloatRect textRect = text.getGlobalBounds();
	text.setPosition(sf::Vector2f(boundingBox.left + (boundingBox.width - textRect.width) / 2.0f, 
								  boundingBox.top + (boundingBox.height - textRect.height - text.getCharacterSize() / 2.0f) / 2.0f));
}

void MenuText::drawSubitem(sf::RenderWindow & window, sf::Text & item, sf::FloatRect & boundBox)
{
	sf::FloatRect intersection;
	sf::FloatRect itemBounds = item.getGlobalBounds();

	if (boundBox.intersects(itemBounds, intersection)) {
		if (intersection.width != itemBounds.width || intersection.height != itemBounds.height) {
			// part of the subitem is off of the menu, so scale it down to keep it in.
			if (intersection.width < itemBounds.width) {
				//item.setScale((intersection.width - borderSpriteSize / 2.0f) / itemBounds.width, 1.0f);
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