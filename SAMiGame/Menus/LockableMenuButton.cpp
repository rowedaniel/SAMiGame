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

std::tuple<int, int> LockableMenuButton::checkMouseUp(sf::Vector2f pos) {
	if (locked) {
		return std::make_tuple(-1, -1);
	}
	return MenuButton::checkMouseUp(pos);
}

void LockableMenuButton::lock()
{
	// TODO: put lock/unlock image distinctions
	std::cout << "button successfully locked!" << std::endl;

	locked = true;
}

void LockableMenuButton::unlock()
{
	locked = false;
}
