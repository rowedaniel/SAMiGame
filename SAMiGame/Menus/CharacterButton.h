#pragma once
#include "../stdafx.h";
#include "MenuButton.h";

class CharacterButton : public MenuButton
{
public:
	void load(std::istream& file);
};
