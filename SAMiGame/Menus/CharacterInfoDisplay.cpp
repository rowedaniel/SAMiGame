#include "CharacterInfoDisplay.h";

CharacterInfoDisplay::CharacterInfoDisplay()
{
}

CharacterInfoDisplay::~CharacterInfoDisplay()
{
}

void CharacterInfoDisplay::setCharacterData(std::vector<Character>::iterator sourceCharacter)
{
	//sprite.setTextureRect(sf::IntRect(type * 32, secondaryType * 32, 32, 32));
	primaryTypeSprite.setTextureRect(sf::IntRect(sourceCharacter->primary_type * 32, 0, 32, 32));
	secondaryTypeSprite.setTextureRect(sf::IntRect(sourceCharacter->secondary_type * 32, 32, 32, 32));
	characterName.setText(sourceCharacter->name);
	characterDescription.setText(sourceCharacter->description);
	characterAttack.setText("Attack: " + std::to_string((int)sourceCharacter->attack));
	//std::cout << "description length: " << sourceCharacter->description.length() << std::endl;
}

void CharacterInfoDisplay::load(std::istream& file)
{
	// TODO: figure out how to integrate this with superclass
	CharacterInfoDisplay::LoadInfo loadInfo;

	// load main text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}
	loadFileData(loadInfo);
	loadTextureData();
}

void CharacterInfoDisplay::draw(sf::RenderWindow & window, sf::FloatRect boundBox)
{
	drawBackground(window, boundBox);

	window.draw(primaryTypeSprite);
	window.draw(secondaryTypeSprite);
	characterName.draw(window, boundBox);
	characterDescription.draw(window, boundBox);
	characterAttack.draw(window, boundBox);

	drawBorder(window, boundBox);

}

void CharacterInfoDisplay::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();
}


void CharacterInfoDisplay::updateItemPos()
{
	primaryTypeSprite.setPosition(sf::Vector2f(boundingBox.left + 16.0f, boundingBox.top + 16.0f));
	secondaryTypeSprite.setPosition(sf::Vector2f(boundingBox.left + 16.0f, boundingBox.top + 48.0f));
	characterName.updatePos(sf::Vector2f(boundingBox.left + 48.0f, boundingBox.top + 16.0f));
	characterDescription.updatePos(sf::Vector2f(boundingBox.left + 170.0f, boundingBox.top + 16.0f));
	characterAttack.updatePos(sf::Vector2f(boundingBox.left + 0.0f, boundingBox.top + 40.0f));
}


void CharacterInfoDisplay::getFileLineData(int i, std::string & line, LoadInfo & loadInfo)
{
	if (i < 6) { // don't bother for early stuff
		MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
		return;
	}
	i -= 5;

	if (i < 7) {
		loadInfo.characterNameText.append(line + "\n");
		return;
	}
	i -= 6;

	if (i < 7) {
		loadInfo.characterOtherText.append(line + "\n");
		return;
	}
	i -= 6;
}

void CharacterInfoDisplay::loadFileData(LoadInfo & loadInfo)
{

	{
		//std::cout << "in characterinfodisplay" << characterRef->name << std::endl;
		std::istringstream textStream(loadInfo.characterNameText + "Test\n");
		characterName.load(textStream);
	}
	{
		//std::cout << "in characterinfodisplay" << characterRef->name << std::endl;
		std::istringstream textStream(loadInfo.characterOtherText + "Test\n");
		characterDescription.load(textStream);
	}
	{
		//std::cout << "in characterinfodisplay" << characterRef->name << std::endl;
		std::istringstream textStream(loadInfo.characterOtherText + "Test\n");
		characterAttack.load(textStream);
	}

	MenuItem::loadFileData(loadInfo.oldInfo);
}

void CharacterInfoDisplay::loadTextureData()
{
	MenuItem::loadTextureData();
	// TODO: move this to resource manager
	if (!typeTexture.loadFromFile("data/images/sprites/types.png")) {
		// error
		std::cout << "error loading typedisplay texture!" << std::endl;
		return;
	}
	primaryTypeSprite.setTexture(typeTexture);
	secondaryTypeSprite.setTexture(typeTexture);
}
