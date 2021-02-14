#pragma once
#include "../stdafx.h"

class CharacterTemplate
{
public:
	CharacterTemplate(int characterType);
	~CharacterTemplate();

	int type; // which character, kevin, etc.
	std::string name;
	std::string description;
	float attack;
	int primary_type; // the actual type. TODO: make this terminology less confusing
	int secondary_type;

	void load();

protected:
	struct LoadInfo
	{
		std::string name;
		std::string description;
		float attack = 0;
		int primary_type = 0;
		int secondary_type = 0;
	};

	void getFileLineData(int i, std::string line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	sf::Texture texture;
};