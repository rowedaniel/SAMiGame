#include "LockableMenuButton.h"

LockableMenuButton::LockableMenuButton()
{
}

LockableMenuButton::~LockableMenuButton()
{
}

void LockableMenuButton::checkMouseDown(sf::Vector2f pos)
{
	if (locked) {
		return;
	}
	MenuButton::checkMouseDown(pos);
}

void LockableMenuButton::lock()
{
	// TODO: put lock/unlock image distinctions
	locked = true;
}

void LockableMenuButton::unlock()
{
	locked = false;
}
