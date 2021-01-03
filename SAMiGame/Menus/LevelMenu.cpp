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
void LevelMenu::updateItemPos()
{
	testButton.updatePos(sf::Vector2f(100.0f, 100.0f));
	boundingBox.width = 1000;
	boundingBox.height = 1000;
}

void LevelMenu::draw(sf::RenderWindow & window)
{
	//MenuItem::draw(window, boundingBox);
	testButton.draw(window, boundingBox);
}
