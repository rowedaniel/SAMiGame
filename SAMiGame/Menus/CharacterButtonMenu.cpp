#include "CharacterButtonMenu.h"

CharacterButtonMenu::CharacterButtonMenu()
{
}

CharacterButtonMenu::~CharacterButtonMenu()
{
}



void CharacterButtonMenu::unload()
{
	characterRefs.clear();
	totalCharacterCount = 0;
}

void CharacterButtonMenu::draw(sf::RenderWindow & window, sf::FloatRect & boundBox, sf::Time elapsedTime)
{
	drawBackground(window, boundBox);

	for (auto it : characterRefs) {
		it->draw(window, boundingBox);
	}
	if (characterOffset != targetCharacterOffset) {
		updateScroll(elapsedTime);
	}

	drawBorder(window, boundBox);
}



void CharacterButtonMenu::addCharacter(std::vector<CharacterButton>::iterator character)
{
	characterRefs.push_back(character);
	++totalCharacterCount;
}

void CharacterButtonMenu::removeCharacter()
{
	characterRefs.pop_back();
}




void CharacterButtonMenu::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();
}

void CharacterButtonMenu::updateItemPos()
{
	float x = boundingBox.left + 4.0f + characterOffset;
	float y = boundingBox.top + 4.0f;
	for (auto it : characterRefs) {
		it->updatePos(sf::Vector2f(x, y));
		x += 32.0f;
	}
}




void CharacterButtonMenu::scrollCharacter(int numberOfCharacters)
{
	const float w = (*characterRefs.begin())->getWidth();
	targetCharacterOffset -= numberOfCharacters * w;
	// don't go too far to the left or right
	targetCharacterOffset = std::min(0.0f, targetCharacterOffset);
	targetCharacterOffset = std::max(boundingBox.width-8.0f-w*totalCharacterCount, targetCharacterOffset);

	// figure out how fast to go, and in what direction
	changeSpeed = (targetCharacterOffset - characterOffset) / 0.1f; // it should 0.1 second to get to the next character
}



void CharacterButtonMenu::updateScroll(sf::Time elapsedTime)
{
	const float expectedNextStep = changeSpeed * elapsedTime.asSeconds();
	if (abs(targetCharacterOffset - characterOffset) < abs(expectedNextStep)) {
		characterOffset = targetCharacterOffset;
		//std::cout << "done!" << std::endl;
	}
	else {

		characterOffset += expectedNextStep;
		//std::cout << "updating character pos, currently at: " << characterOffset << " of: " << targetCharacterOffset << std::endl;
	}
	updateItemPos();
	for (auto it : characterRefs) {
		it->resetScale();
	}
}