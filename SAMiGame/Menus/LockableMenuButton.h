#pragma once
#include "../stdafx.h"
#include "MenuButton.h"

class LockableMenuButton : public MenuButton
{
public:
	LockableMenuButton();
	~LockableMenuButton();

	void checkMouseDown(sf::Vector2f pos);

	void lock();
	void unlock();

private:
	bool locked = true;
};