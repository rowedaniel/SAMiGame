#include "CharacterButton.h"

void CharacterButton::load(std::istream & file)
{
	CharacterButton::LoadInfo loadInfo;

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
}

void CharacterButton::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	//MenuItem::draw(window, boundBox);
	drawSubitem(window, sprite, boundBox);
}

void CharacterButton::updatePos(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

void CharacterButton::getFileLineData(int i, std::string line, LoadInfo & loadInfo)
{
	MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
}

void CharacterButton::loadFileData(LoadInfo & loadInfo)
{
	MenuItem::loadFileData(loadInfo.oldInfo);
	std::cout << "kevin of color: red-" << loadInfo.oldInfo.red << ", green-" << loadInfo.oldInfo.green << ", blue-" << loadInfo.oldInfo.blue << std::endl;
	sprite.setColor(sf::Color(loadInfo.oldInfo.red, loadInfo.oldInfo.green, loadInfo.oldInfo.blue));
}

void CharacterButton::loadTextureData()
{
	//MenuItem::loadTextureData();
	
	if (!texture.loadFromFile("data/images/sprites/Kevin.png")) {
		// error
		return;
	}

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));

}
