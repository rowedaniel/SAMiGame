#include "CharacterTemplate.h";

CharacterTemplate::CharacterTemplate(int characterType)
{
	type = characterType;
	std::cout << "new character template of type: " << characterType << std::endl;
}

CharacterTemplate::~CharacterTemplate()
{
}

void CharacterTemplate::load()
{
	std::fstream file("data/characters/"+std::to_string(type)+".characterTemplate", std::ios::in);
	if (!file.is_open()) {
		// Error
		// TODO: figure out error handling
		std::cout << "Error in opening file: " << "data/characters/" + std::to_string(type) + ".characterTemplate" << std::endl;
		return;
	}

	LoadInfo loadInfo;

	// load text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}

	// do stuff with the file data
	loadFileData(loadInfo);
	loadTextureData();
}

void CharacterTemplate::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	// TODO: copy parts of this into menu button class
	switch (i) {
	case 1:
	{
		loadInfo.name = line;
		break;
	}
	case 2:
	{
		loadInfo.description = line;
		break;
	}
	case 3:
	{
		loadInfo.attack = std::stof(line);
		break;
	}
	case 4:
	{
		loadInfo.primary_type = std::stoi(line);
		break;
	}
	case 5:
	{
		loadInfo.secondary_type = std::stoi(line);
		break;
	}
	}
}

void CharacterTemplate::loadFileData(LoadInfo & loadInfo)
{
	name = loadInfo.name;
	description = loadInfo.description;
	attack = loadInfo.attack;
	primary_type = loadInfo.primary_type;
	secondary_type = loadInfo.secondary_type;
}

void CharacterTemplate::loadTextureData()
{
	if (!texture.loadFromFile("data/images/sprites/characters/" + std::to_string(type) + ".png")) {
		// error
		std::cout << "error loading character texture: data/images/sprites/characters/" + std::to_string(type) + ".png" << std::endl;
		return;
	}
}

