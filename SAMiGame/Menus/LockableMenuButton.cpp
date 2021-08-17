#include "LockableMenuButton.h"

LockableMenuButton::LockableMenuButton()
{
}

LockableMenuButton::LockableMenuButton(bool startingState)
{
	locked = startingState;
	LockableMenuButton();
}

LockableMenuButton::~LockableMenuButton()
{
}








void LockableMenuButton::load(std::istream & file)
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

void LockableMenuButton::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	if (i < 10) {
		MenuButton::getFileLineData(i, line, loadInfo.oldInfo);
		return;
	}
	i -= 9;

	loadInfo.id = std::stoi(line);
}

void LockableMenuButton::loadFileData(LoadInfo & loadInfo)
{
	MenuButton::loadFileData(loadInfo.oldInfo);
	unlockedColor = background.getFillColor();
	id = loadInfo.id;
	if (id != -1) {
		lock();
	}
}







void LockableMenuButton::checkMouseDown(sf::Vector2f pos)
{
	if (locked) {
		return;
	}
	MenuButton::checkMouseDown(pos);
}

std::tuple<int, int> LockableMenuButton::checkMouseUp(sf::Vector2f pos) {
	if (locked) {
		return std::make_tuple(-1, -1);
	}
	return MenuButton::checkMouseUp(pos);
}





void LockableMenuButton::lock()
{
	// TODO: put lock/unlock image distinctions
	background.setFillColor(lockedColor);
	locked = true;
}

void LockableMenuButton::unlock()
{
	background.setFillColor(unlockedColor);
	locked = false;
}

int LockableMenuButton::getId()
{
	return id;
}
