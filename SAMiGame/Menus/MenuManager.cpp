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


	const sf::Vector2f pos = sf::Vector2f(top, left);
	activeLevel.updatePos(pos);
	characterSelect.updatePos(pos);

	activeMenu = menus.begin();
}

void MenuManager::draw(sf::RenderWindow & window, sf::Time elapsedTime)
{
	// TODO: figure out how to do this without the ugly if's
	switch (state)
	{
	case InMenu:
	{
		activeMenu->draw(window);
		break;
	}
	case InLevel:
	{
		activeLevel.draw(window, elapsedTime);
		break;
	}
	case InCharacterSelect:
	{
		characterSelect.draw(window, elapsedTime);
		break;
	}
	}
}

void MenuManager::checkMouseDown(sf::Vector2f pos)  // checks for buttons being clicked
{

	switch (state)
	{
	case InMenu:
	{
		activeMenu->checkMouseDown(pos);
		break;
	}
	case InLevel:
	{
		activeLevel.checkMouseDown(pos);
		break;
	}
	case InCharacterSelect:
	{
		characterSelect.checkMouseDown(pos);
		break;
	}
	}
}


void MenuManager::checkMouseUp(sf::Vector2f pos)  // checks for buttons being clicked
{
	switch (state)
	{
	case InMenu:
	{
		int actionType, actionArg;
		std::tie(actionType, actionArg) = activeMenu->checkMouseUp(pos);
		std::cout << "recieved actionType: " << actionType << ", actionArg: " << actionArg << std::endl;
		executeButton(actionType, actionArg);
		break;
	}
	case InLevel:
	{
		//std::tie(actionType, actionArg) = activeLevel.checkMouseUp(pos);
		// TODO: make this work. For now, just run it.
		activeLevel.checkMouseUp(pos);
		if (activeLevel.isDone()) {
			stopLevel();
		}
		break;
	}
	case InCharacterSelect:
	{
		characterSelect.checkMouseUp(pos);
		if (characterSelect.isDone()) {
			// TOOD: stop character select here
			state = InMenu;
		}
	}
	}
}

void MenuManager::checkMouseMove(sf::Vector2f pos)
{
	switch(state)
	{
	case InMenu:
	{
		activeMenu->checkMouseMove(pos);
		break;
	}
	case InLevel:
	{
		activeLevel.checkMouseMove(pos);
		break;
	}
	case InCharacterSelect:
	{
		characterSelect.checkMouseMove(pos);
		break;
	}
	}
}

void MenuManager::executeButton(int actionType, int actionArg)
{
	if (state != InMenu) {
		// no buttoning unless in menus!
		return;
	}
	switch (static_cast<MenuManager::ActionType>(actionType)) {
	case Swapmenu:
	{
		swapMenus(actionArg);
		break;
	}
	case StartLevel:
	{
		startLevel(actionArg);
		break;
	}
	case StartCharacterSelect:
	{
		startCharacterSelect(actionArg);
		break;
	}
	}
}

void MenuManager::swapMenus(int id)
{
	if (state != InMenu) {
		std::cout << "Tried to swap menus while not in menu. Not sure how to handle this." << std::endl;
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
	// TODO: unload all the unnecessary level stuff when starting level
	if (activeLevel.load(id)) {
		state = InLevel;
	}
}

void MenuManager::stopLevel()
{
	// TODO: load all the level stuff when ending level
	// unlock new levels and stuff
	const int buttonId = activeLevel.getButtonToUnlock();
	if(buttonId != 0){
		for (auto it = menus.begin(); it != menus.end(); ++it) {
			it->unlockButton(buttonId);
		}
	}

	activeLevel.unload();
	state = InMenu;
}

void MenuManager::startCharacterSelect(int numberOfCharacters)
{
	if (characterSelect.load(numberOfCharacters)) {
		std::cout << "starting character select" << std::endl;
		state = InCharacterSelect;
	}
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
