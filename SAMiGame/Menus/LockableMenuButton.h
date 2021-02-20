#pragma once
#include "../stdafx.h"
#include "MenuButton.h"

class LockableMenuButton : public MenuButton
{
public:
	LockableMenuButton();
	~LockableMenuButton();

	std::tuple<int, int> checkMouseUp(sf::Vector2f pos);
	void checkMouseDown(sf::Vector2f pos);

	void lock();
	void unlock();

private:
	bool locked = true;
};