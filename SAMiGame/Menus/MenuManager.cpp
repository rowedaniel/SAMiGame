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
		if (line.substr(0, 2) == "//") {
			continue;
		}
		//std::cout << "line " << i+1 << ": " << line << std::endl;
		getFileLineData(++i, line, loadInfo);
	}

	// load all the menus
	menus.reserve(loadInfo.totalNumberOfMenus);
	std::vector<Menu>::iterator latestMenu;
	bool firstMenu = false;
	for (auto it = loadInfo.menuText.begin(); it != loadInfo.menuText.end(); ++it) {
		// load submenus (recursion is sweet)!
		//std::cout << "Menu text: " << std::endl << it->substr() << std::endl;
		std::istringstream textStream(it->substr());
		menus.push_back(Menu::Menu(left, top));
		//menus.back().load(textStream);
		if (!firstMenu) {
			latestMenu = menus.begin();
			firstMenu = true;
		}
		else {
			++latestMenu;
		}
		latestMenu->load(textStream);
	}


	std::fstream unlockedButtonFile("gamestate/unlocked.buttons", std::ios::in);
	if (unlockedButtonFile.is_open()) {
		std::string line;
		int i = 0;
		while (getline(unlockedButtonFile, line)) {
			if (line.substr(0, 2) == "//") {
				continue;
			}
			for (auto it = menus.begin(); it != menus.end(); ++it) {
				// TODO: make this more crash-secure
				it->unlockButton(std::stoi(line));
			}
		}
	}



	activeLevel.updatePos(sf::Vector2f(top, left));
	activeMenu = menus.begin();
}

void MenuManager::draw(sf::RenderWindow & window, sf::Time elapsedTime)
{
	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		activeLevel.draw(window, elapsedTime);
	}
	else {
		activeMenu->draw(window);
	}
}

void MenuManager::checkMouseDown(sf::Vector2f pos)  // checks for buttons being clicked
{

	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		activeLevel.checkMouseDown(pos);
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
		// TODO: make this work. For now, just run it.
		activeLevel.checkMouseUp(pos);
		if (activeLevel.isDone()) {
			stopLevel();
		}
		return;
	}
	else {
		std::tie(actionType, actionArg) = activeMenu->checkMouseUp(pos);
	}
	std::cout << "recieved actionType: " << actionType << ", actionArg: " << actionArg << std::endl;
	executeButton(actionType, actionArg);
}

void MenuManager::checkMouseMove(sf::Vector2f pos)
{
	// TODO: figure out how to do this without the ugly if's
	if (inLevel) {
		activeLevel.checkMouseMove(pos);
	}
	else {
		activeMenu->checkMouseMove(pos);
	}
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
	for (auto it = menus.begin(); it != menus.end(); ++it) {
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

	if (activeLevel.load(id)) {
		inLevel = true;
	}
}

void MenuManager::stopLevel()
{
	activeLevel.unload();
	inLevel = false;
}

void MenuManager::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	switch (i) {
	case 1: 
	{
		loadInfo.numberOfMenus = std::stoi(line);
		loadInfo.totalNumberOfMenus = loadInfo.numberOfMenus;
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
