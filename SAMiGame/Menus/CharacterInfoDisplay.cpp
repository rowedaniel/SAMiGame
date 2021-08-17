#include "CharacterInfoDisplay.h"

CharacterInfoDisplay::CharacterInfoDisplay()
{
}

CharacterInfoDisplay::~CharacterInfoDisplay()
{
}

void CharacterInfoDisplay::setCharacterData(std::vector<Character>::iterator sourceCharacter)
{
	std::cout << "displaying character data!" << std::endl;

	// display character primary and secondary types
	primaryTypeSprite.setTextureRect(sf::IntRect(sourceCharacter->primary_type * 32, 0, 32, 32));
	secondaryTypeSprite.setTextureRect(sf::IntRect(sourceCharacter->secondary_type * 32, 32, 32, 32));

	// display various character stats
	characterName.setText(sourceCharacter->name);
	// TODO: figure out how to display actual effect info
	characterAttack.setText("Level: " + std::to_string(sourceCharacter->getLevel()) + \
							"   Attack: " + std::to_string((int)std::round(sourceCharacter->getAttack())) + 
							"   Effect cooldown: " + std::to_string(sourceCharacter->getEffectCooldown())
							);





	// display effects that it causes
	// get effects
	std::list<Effect> effects;
	for (auto it = sourceCharacter->selfAppliedEffects.begin(); it != sourceCharacter->selfAppliedEffects.end(); ++it) {
		effects.push_back(getEffectInfo(it, sourceCharacter->type, sourceCharacter->isPlayerOwned()));
	}
	for (auto it = sourceCharacter->opponentAppliedEffects.begin(); it != sourceCharacter->opponentAppliedEffects.end(); ++it) {
		effects.push_back(getEffectInfo(it, sourceCharacter->type, !sourceCharacter->isPlayerOwned()));
	}
	for (auto it = sourceCharacter->selfPlayerAppliedEffects.begin(); it != sourceCharacter->selfPlayerAppliedEffects.end(); ++it) {
		effects.push_back(getEffectInfo(it, sourceCharacter->type, sourceCharacter->isPlayerOwned()));
	}
	for (auto it = sourceCharacter->opponentPlayerAppliedEffects.begin(); it != sourceCharacter->opponentPlayerAppliedEffects.end(); ++it) {
		effects.push_back(getEffectInfo(it, sourceCharacter->type, !sourceCharacter->isPlayerOwned()));
	}
	loadEffectButton(effects, causedEffects);




	// display active effects on character
	auto activeEffectInfo = sourceCharacter->getEffects();
	loadEffectButton(activeEffectInfo, activeEffects);



	updateItemPos();

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

	// draw sprites first
	window.draw(primaryTypeSprite);
	window.draw(secondaryTypeSprite);

	// draw text second so it goes on top
	characterName.draw(window, boundBox);
	//characterDescription.draw(window, boundBox);
	characterAttack.draw(window, boundBox);

	// draw effects last, so the text shows up on top
	// TODO: make it so the text doesn't get blocked by other effects
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		it->draw(window, boundingBox);
	}
	for (auto it = causedEffects.begin(); it != causedEffects.end(); ++it) {
		it->draw(window, boundingBox);
	}
	
	
	drawBorder(window, boundBox);

}

void CharacterInfoDisplay::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();
}


void CharacterInfoDisplay::updateItemPos()
{
	characterName.updatePos(sf::Vector2f(boundingBox.left + 48.0f, boundingBox.top + 16.0f));
	//characterDescription.updatePos(sf::Vector2f(boundingBox.left + 170.0f, boundingBox.top + 16.0f));
	characterAttack.updatePos(sf::Vector2f(boundingBox.left + 170.0f, boundingBox.top + 16.0f));
	float x = boundingBox.left + 16.0f;
	primaryTypeSprite.setPosition(sf::Vector2f(x, boundingBox.top + 16.0f));
	secondaryTypeSprite.setPosition(sf::Vector2f(x, boundingBox.top + 48.0f));

	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		it->updatePos(sf::Vector2f(x, boundingBox.top + 96.0f));
	}

	x += 64;
	for (auto it = causedEffects.begin(); it != causedEffects.end(); ++it) {
		it->updatePos(sf::Vector2f(x, boundingBox.top + 64.0f));
	}
}

void CharacterInfoDisplay::checkMouseMove(sf::Vector2f pos)
{
	for (auto it = causedEffects.begin(); it != causedEffects.end(); ++it) {
		it->checkMouseMove(pos);
	}
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		it->checkMouseMove(pos);
	}
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
	/*
	{
		//std::cout << "in characterinfodisplay" << characterRef->name << std::endl;
		std::istringstream textStream(loadInfo.characterOtherText + "Test\n");
		characterDescription.load(textStream);
	}
	*/
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

Effect CharacterInfoDisplay::getEffectInfo(std::vector<EffectGetter::EffectInfo>::iterator effect, int characterType, bool playerOwned)
{
	return EffectGetter::getEffect(*effect, characterType, playerOwned);
}

void CharacterInfoDisplay::loadEffectButton(std::list<Effect> effectList, std::list<EffectButton>& effectButtonList)
{
	effectButtonList.clear();
	for (auto it = effectList.begin(); it != effectList.end(); ++it) {
		effectButtonList.push_back(EffectButton());
	}
	auto info = effectList.begin();
	for (auto it = effectButtonList.begin(); it != effectButtonList.end(); ++it) {
		it->load();
		it->setEffectData(info);
		++info;
	}
}
