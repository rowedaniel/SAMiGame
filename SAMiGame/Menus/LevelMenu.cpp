#include "LevelMenu.h"

LevelMenu::LevelMenu()
{
	LevelMenu(0.0f, 0.0f);
}

LevelMenu::LevelMenu(float x, float y)
{
	boundingBox.left = x;
	boundingBox.top = y;
}

LevelMenu::~LevelMenu()
{
}

void LevelMenu::load(std::istream& file)
{
	// TODO: figure out how to integrate this with superclass
	LevelMenu::LoadInfo loadInfo;

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

	// now, load everything else
	loadTextureData();

	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
	updateItemPos();
}

void LevelMenu::draw(sf::RenderWindow & window)
{

	drawBackground(window, boundingBox);

	for (std::list<CharacterButton>::iterator it = characters.begin(); it != characters.end(); ++it) {
		it->draw(window, boundingBox);
	}

	drawBorder(window, boundingBox);
}




void LevelMenu::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
}

void LevelMenu::updateItemPos()
{
	float x = 100.0f;
	for (std::list<CharacterButton>::iterator it = characters.begin(); it != characters.end(); ++it) {
		it->updatePos(sf::Vector2f(x, 160.0f));
		std::cout << "character positioned at: " << x << std::endl;
		x += 64.0f + 8.0f;
	}
	boundingBox.width = 1000;
	boundingBox.height = 1000;
}

void LevelMenu::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
	if (i < 6) { // don't bother for early stuff
		return;
	}
	switch (i) {
	case 6:
	{
		loadInfo.numberOfCharacters = std::stoi(line);
		std::cout << loadInfo.numberOfCharacters << " kevins to be made!" << std::endl;
		break;
	}
	default:
	{
		if (loadInfo.numberOfCharacters > 0) {
			if (loadInfo.extraLines <= 0) {
				loadInfo.extraLines = std::stoi(line);
				loadInfo.characterText.push_back(std::string());
			}
			else {
				loadInfo.characterText.back().append(line + "\n");
				if (--loadInfo.extraLines <= 0) {
					--loadInfo.numberOfCharacters;
					std::cout << "Recieved data for a kevin. " << loadInfo.numberOfCharacters << " left" << std::endl;
				}
			}
		}
		break;
	}
	}
}

void LevelMenu::loadFileData(LoadInfo & loadInfo)
{
	for (std::list<std::string>::iterator it = loadInfo.characterText.begin(); it != loadInfo.characterText.end(); ++it) {
		// load characters!
		std::istringstream textStream(it->substr());
		characters.push_back(CharacterButton::CharacterButton());
		characters.back().load(textStream);
	}

	MenuItem::loadFileData(loadInfo.oldInfo);
}