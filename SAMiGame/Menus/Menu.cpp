#include "../stdafx.h"
#include "Menu.h"
#include <string>
#include <sstream>

Menu::Menu(float x, float y)
{
	//rectSourceSprite = sf::IntRect(0, 0, width, height);

	// TODO: this is temporary, fix later
	//sprite = sf::RectangleShape(sf::Vector2f(rectSourceSprite.width, rectSourceSprite.height));
	boundingBox.left = x;
	boundingBox.top = y;


	/*buttons = subButtons;
	menus = subMenus;

	direction = dir;*/
}

Menu::~Menu()
{
}

void Menu::load(std::istream& file)
{
	// TODO: figure out how to integrate this with superclass
	Menu::LoadInfo loadInfo;

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
	updateItemPos();
}

void Menu::draw(sf::RenderWindow & window)
{
	draw(window, boundingBox);
}

void Menu::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	if (!boundBox.intersects(boundingBox)) {
		return; // not in the menu, so don't draw.
	}

	drawBackground(window, boundBox);

	// draw all submenus
	for (std::list<Menu>::iterator it = menus.begin(); it != menus.end(); ++it) {
		it->draw(window, boundBox);
	}


	// draw all text
	for (std::list<MenuText>::iterator it = texts.begin(); it != texts.end(); ++it) {
		it->draw(window, boundBox);
	}

	// draw all buttons
	for (std::list<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		it->draw(window, boundBox);
	}

	drawBorder(window, boundBox);
}





void Menu::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();

}




void Menu::updateItemPos()
{
	// TODO: figure out how to compact this
	int itemScroll; // how far the item is adjusted at any particular moment
	// these two get set early, and then don't change. They're used to determine scroll direction:
	float itemScrollXAmount = 0.0f;
	float itemScrollYAmount = 0.0f;
	switch (direction)
	{
	case Right:
	{
		itemScroll = borderSpriteSize; // start from left side - a little bit of extra space
		itemScrollXAmount = 1.0f; // go right
		break;
	}
	case Left:
	{
		itemScroll = (int)boundingBox.width - borderSpriteSize; // start from right side - a little bit of extra space
		itemScrollXAmount = -1.0f; // go left
		break;
	}
	case Down:
	{
		itemScroll = borderSpriteSize; // start from top side - a little bit of extra space
		itemScrollYAmount = 1.0f; // go down
		break;
	}
	case Up:
	{
		itemScroll = (int)boundingBox.height- borderSpriteSize; // start from bottom side - a little bit of extra space
		itemScrollYAmount = -1.0f; // go up
		break;
	}
	}
	
	// First, position all the text
	for (std::list<MenuText>::iterator it = texts.begin(); it != texts.end(); ++it) {
		// Increment scroll, so items are offset
		const float scrollAmount = (it->getWidth() * itemScrollXAmount + it->getHeight() * itemScrollYAmount);
		itemScroll += scrollAmount < 0 ? (int)scrollAmount - 4 : 0; // the -4 adds space in between items.

		// Position item at x = middle of menu, y = bottom of previous item
		it->updatePos(
			sf::Vector2f(
				boundingBox.left + (itemScrollXAmount == 0.0f ? (boundingBox.width - it->getWidth()) / 2.0f : itemScroll),
				boundingBox.top + (itemScrollYAmount == 0.0f ? (boundingBox.height - it->getHeight()) / 2.0f : itemScroll)
			));

		// Increment scroll, so items are offset
		itemScroll += scrollAmount > 0 ? (int)scrollAmount + 4 : 0; // the +4 adds space in between items.
	}

	// Next, position all the buttons
	for (std::list<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		// Increment scroll, so items are offset
		const float scrollAmount = (it->getWidth() * itemScrollXAmount + it->getHeight() * itemScrollYAmount);
		itemScroll += scrollAmount < 0 ? (int)scrollAmount - 4 : 0; // the -4 adds space in between items.

		// Position item at x = middle of menu, y = bottom of previous item
		it->updatePos(
			sf::Vector2f(
			boundingBox.left + (itemScrollXAmount == 0.0f ? (boundingBox.width  - it->getWidth())  / 2.0f : itemScroll),
			boundingBox.top  + (itemScrollYAmount == 0.0f ? (boundingBox.height - it->getHeight()) / 2.0f : itemScroll)
			));

		// Increment scroll, so items are offset
		itemScroll += scrollAmount > 0 ? (int)scrollAmount + 4: 0; // the +4 adds space in between items.
	}

	// Then, position all the menus
	for (std::list<Menu>::iterator it = menus.begin(); it != menus.end(); ++it) {
		// Increment scroll, so items are offset
		const float scrollAmount = (it->getWidth() * itemScrollXAmount + it->getHeight() * itemScrollYAmount);
		itemScroll += scrollAmount < 0 ? (int)scrollAmount - 4 : 0; // the -4 adds space in between items.

		// Position item at x = middle of menu, y = bottom of previous item
		it->updatePos(
			sf::Vector2f(
				boundingBox.left + (itemScrollXAmount == 0.0f ? (boundingBox.width  - it->getWidth())  / 2.0f : itemScroll),
				boundingBox.top  + (itemScrollYAmount == 0.0f ? (boundingBox.height - it->getHeight()) / 2.0f : itemScroll)
			));
		it->updateItemPos();

		// Increment scroll, so items are offset
		itemScroll += scrollAmount > 0 ? (int)scrollAmount + 4 : 0; // the +4 adds space in between items.
	}
}

void Menu::checkMouseDown(sf::Vector2f pos) // checks for buttons being clicked
{
	if (!boundingBox.contains(pos)) {
		return;
	}

	// check all submenus
	for (std::list<Menu>::iterator it = menus.begin(); it != menus.end(); ++it) {
		it->checkMouseDown(pos);
	}

	// check all buttons
	for (std::list<MenuButton>::iterator it = buttons.begin(); it != buttons.end(); ++it) {
		it->checkMouseDown(pos);
	}
}

std::tuple<int, int> Menu::checkMouseUp(sf::Vector2f pos)
{

	// this result indicates the button wasn't clicked
	std::tuple<int, int> nullResult = { -1, -1 };



	if (!boundingBox.contains(pos)) {
		return nullResult;
	}


	std::tuple<int, int> result;

	// check all submenus
	for (auto it = menus.begin(); it != menus.end(); ++it) {
		result = it->checkMouseUp(pos);
		if (result != nullResult) {
			return result;
		}
	}

	// check all buttons
	for (auto it = buttons.begin(); it != buttons.end(); ++it) {
		result = it->checkMouseUp(pos);
		if (result != nullResult) {
			return result;
		}
	}
	return nullResult;
}

void Menu::checkMouseMove(sf::Vector2f pos)
{
}






void Menu::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
	if (i < 6) { // don't bother for early stuff
		return;
	}
	switch (i) {
	case 6: 
	{
		loadInfo.id = std::stoi(line);
	}
	case 7:
	{
		loadInfo.dir = std::stoi(line);
		break;
	}
	case 8:
	{
		loadInfo.numberOfTexts = std::stoi(line);
		break;
	}
	case 9:
	{
		loadInfo.numberOfButtons = std::stoi(line);
		break;
	}
	case 10:
	{
		loadInfo.numberOfSubmenus = std::stoi(line);
		break;
	}
	default:
	{
		if (loadInfo.numberOfTexts > 0) {
			if (loadInfo.extraLines <= 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.textText.push_back(std::string());
			}
			else {
				loadInfo.textText.back().append(line + "\n");
				if (--loadInfo.extraLines <= 0) {
					--loadInfo.numberOfTexts;
				}
			}
		}
		else if (loadInfo.numberOfButtons > 0) {
			if (loadInfo.extraLines <= 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.buttonText.push_back(std::string());
			}
			else {
				loadInfo.buttonText.back().append(line + "\n");
				if (--loadInfo.extraLines <= 0) {
					--loadInfo.numberOfButtons;
				}
			}
		}
		else if (loadInfo.numberOfSubmenus > 0) {
			if (loadInfo.extraLines <= 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.submenuText.push_back(std::string());
			}
			else {
				loadInfo.submenuText.back().append(line + "\n");
				if (--loadInfo.extraLines <= 0) {
					--loadInfo.numberOfSubmenus;
				}
			}
		}
		break;
	}
	}
}

void Menu::loadFileData(LoadInfo & loadInfo)
{
	// TODO: put parts of this into parent class for both menu buttons and menus

	direction = static_cast<Menu::Direction>(loadInfo.dir);
	id = loadInfo.id;

	for (std::list<std::string>::iterator it = loadInfo.textText.begin(); it != loadInfo.textText.end(); ++it) {
		// load buttons!
		std::istringstream textStream(it->substr());
		texts.push_back(MenuText::MenuText());
		texts.back().load(textStream);
	}
	for (std::list<std::string>::iterator it = loadInfo.buttonText.begin(); it != loadInfo.buttonText.end(); ++it) {
		// load buttons!
		std::istringstream textStream(it->substr());
		buttons.push_back(MenuButton::MenuButton());
		buttons.back().load(textStream);
	}
	for (std::list<std::string>::iterator it = loadInfo.submenuText.begin(); it != loadInfo.submenuText.end(); ++it) {
		// load submenus (recursion is sweet)!
		std::istringstream textStream(it->substr());
		menus.push_back(Menu::Menu(0.0f, 0.0f));
		menus.back().load(textStream);
	}
	// End debug section



	// now that we have the data, actually set the values
	MenuItem::loadFileData(loadInfo.oldInfo);
}