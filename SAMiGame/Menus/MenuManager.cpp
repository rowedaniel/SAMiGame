#include "../stdafx.h"
#include "MenuManager.h"
#include <string>
#include <sstream>

MenuManager::MenuManager(float x, float y)
{
	top = y;
	left = x;
}

MenuManager::~MenuManager()
{
}

void MenuManager::load(std::istream & file)
{
	MenuManager::LoadInfo loadInfo;

	// load text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		// DEBUG:

		if (line.substr(0, 2) == "//") {
			continue;
		}
		std::cout << "line " << i+1 << ": " << line << std::endl;
		getFileLineData(++i, line, loadInfo);
	}

	// load all the menus
	for (std::list<std::string>::iterator it = loadInfo.menuText.begin(); it != loadInfo.menuText.end(); ++it) {
		// load submenus (recursion is sweet)!
		std::cout << "Menu text: " << std::endl << it->substr() << std::endl;
		std::istringstream textStream(it->substr());
		menus.push_back(Menu::Menu(left, top));
		menus.back().load(textStream);
	}

	activeMenu = menus.begin();
}

void MenuManager::draw(sf::RenderWindow & window)
{
	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		activeLevel.draw(window);
	}
	else {
		activeMenu->draw(window);
	}
}

void MenuManager::checkMouseDown(sf::Vector2f pos)  // checks for buttons being clicked
{

	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		//activeLevel.checkMouseDown(pos);
	}
	else {
		activeMenu->checkMouseDown(pos);
	}
}


void MenuManager::checkMouseUp(sf::Vector2f pos)  // checks for buttons being clicked
{
	int actionType, actionArg;
	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		//std::tie(actionType, actionArg) = activeLevel.checkMouseUp(pos);
		// TODO: make this work. For now, just quit.
		return;
	}
	else {
		std::tie(actionType, actionArg) = activeMenu->checkMouseUp(pos);
	}
	std::cout << "recieved actionType: " << actionType << ", actionArg: " << actionArg << std::endl;
	executeButton(actionType, actionArg);
}

void MenuManager::executeButton(int actionType, int actionArg)
{
	switch (static_cast<MenuManager::ActionType>(actionType)) {
	case MenuManager::Swapmenu:
	{
		swapMenus(actionArg);
		break;
	}
	case MenuManager::StartLevel:
	{
		startLevel(actionArg);
		break;
	}
	}
}

void MenuManager::swapMenus(int id)
{
	if (inLevel) {
		std::cout << "Tried to swap menus while still in level. Not sure how to handle this." << std::endl;
	}
	for (std::list<Menu>::iterator it = menus.begin(); it != menus.end(); ++it) {
		if (it->id == id) {
			activeMenu = it;
			std::cout << "swapped menus!" << std::endl;
			return;
		}
		else {
			std::cout << "didn't swap, id is: " << it->id << std::endl;
		}
	}
}

void MenuManager::startLevel(int id)
{
	std::ostringstream filename;
	filename << "data/levels/" << id << ".level";
	std::fstream file;

	file.open(filename.str(), std::ios::in);
	if (file.is_open()) {
		inLevel = true;
		activeLevel.updatePos(sf::Vector2f(top, left));
		activeLevel.load(file);
		std::cout << "loaded Level " << id << " filename: " << filename.str() << std::endl;
	}
	else {
		std::cout << "error opening file" << std::endl;
	}
}

void MenuManager::stopLevel()
{
	inLevel = false;
}

void MenuManager::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	switch (i) {
	case 1: 
	{
		loadInfo.numberOfMenus = std::stoi(line);
		break;
	}
	default:
	{
		if (loadInfo.numberOfMenus > 0) {
			if (loadInfo.extraLines <= 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.menuText.push_back(std::string());
				std::cout << "new menu! " << loadInfo.extraLines << " extra lines!" << std::endl;
			}
			else {
				loadInfo.menuText.back().append(line + "\n");
				if (--loadInfo.extraLines <= 0) {
					--loadInfo.numberOfMenus;
				}
			}
		}
		break;
	}
	}
}
