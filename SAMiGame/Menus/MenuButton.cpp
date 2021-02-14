#include "MenuButton.h"

MenuButton::MenuButton()
{
}

MenuButton::~MenuButton()
{
}


void MenuButton::load(std::istream& file)
{
	MenuButton::LoadInfo loadInfo;

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

void MenuButton::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	MenuItem::draw(window, boundBox);
	MenuText::draw(window, boundBox);
}



void MenuButton::checkMouseDown(sf::Vector2f pos)
{
	if (boundingBox.contains(pos)) {
		selected = true;
		std::cout << "button selected!" << std::endl;
	}
	else {
		selected = false;
	}
}

std::tuple<int, int> MenuButton::checkMouseUp(sf::Vector2f pos)
{
	if (selected && boundingBox.contains(pos)) {
		std::cout << "Execute Order " << actionType << ", with arg: " << actionArg << std::endl;
		return std::make_tuple(actionType, actionArg);
	}
	selected = false;
	return std::make_tuple(-1, -1);
}




void MenuButton::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	MenuText::getFileLineData(i, line, loadInfo.oldInfo);
	if (i < 8) { // don't bother for early stuff
		return;
	}
	switch (i) {
	case 8:
	{
		loadInfo.actionType = std::stoi(line);
		break;
	}
	case 9:
	{
		loadInfo.actionArg = std::stoi(line);
		break;
	}
	}
}

void MenuButton::loadFileData(LoadInfo & loadInfo)
{
	actionType = static_cast<MenuButton::Action>(loadInfo.actionType);
	actionArg = loadInfo.actionArg;
	MenuText::loadFileData(loadInfo.oldInfo);
}