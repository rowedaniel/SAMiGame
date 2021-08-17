#pragma once
#include "../stdafx.h"
#include "MenuButton.h"

class LockableMenuButton : public MenuButton
{
public:
	LockableMenuButton();
	LockableMenuButton(bool startingState);
	~LockableMenuButton();


	void load(std::istream& file);


	void checkMouseDown(sf::Vector2f pos);
	std::tuple<int, int> checkMouseUp(sf::Vector2f pos);

	void lock();
	void unlock();

	int getId();

private:
	struct LoadInfo
	{
	public:
		MenuButton::LoadInfo oldInfo;
		int id = -1;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);


	bool locked = true;
	int id;

	const sf::Color lockedColor = sf::Color(80, 80, 80);
	sf::Color unlockedColor;
};