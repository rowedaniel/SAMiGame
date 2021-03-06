#include "CharacterSelect.h"

CharacterSelect::CharacterSelect()
{
}

CharacterSelect::~CharacterSelect()
{
}














bool CharacterSelect::load(int numberOfCharacters)
{

	std::fstream file;
	file.open("data/characterSelect.menuItem", std::ios::in);
	if (!file.is_open())
	{
		std::cout << "error opening file: data/characterSelect.menuItem" << std::endl;
		return false;
	}

	// TODO: figure out how to integrate this with superclass
	CharacterSelect::LoadInfo loadInfo;
	loadInfo.numberOfOutputCharacters = numberOfCharacters;

	// load main text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}


	{
		// load all available characters
		std::fstream file;
		file.open("gamestate/available.characters", std::ios::in);


		if (file.is_open())
		{
			std::cout << "file is open" << std::endl;
			std::string line;
			while (getline(file, line)) {
				// TODO: make this more crash-secure
				loadInfo.characterIds.push_back(std::stoi(line));
				std::cout << "added character" << std::endl;
				++(loadInfo.numberOfCharacters);
			}

			
		}
	}

	if (loadFileData(loadInfo)) {
		loadTextureData();
		updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
		return true;
	}
	return false;



}

void CharacterSelect::unload()
{
}


void CharacterSelect::getFileLineData(int i, std::string & line, LoadInfo & loadInfo)
{
	if (i < 6) { // info for the background
		MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
		return;
	}
	i -= 5;

	if (i < 10)
	{
		loadInfo.exitButtonText.append(line + "\n");
		return;
	}
	i -= 9;

	if (i < 6) {
		loadInfo.characterButtonMenuText.append(line + "\n");
		if (i > 1) {
			// selected Character Button should have a different size
			loadInfo.selectedCharacterButtonMenuText.append(line + "\n");
		}
		return;
	}
	i -= 5;

	if (i < 24) {
		loadInfo.characterInfoMenuText.append(line + "\n");
		return;
	}
	i -= 23;

	if (i < 10) {
		loadInfo.buttonText.append(line + "\n");
		return;
	}
	i -= 9;

	if (i < 10)
	{
		loadInfo.goButtonText.append(line + "\n");
		return;
	}
	i -= 9;
}

bool CharacterSelect::loadFileData(LoadInfo & loadInfo)
{


	// reset anything (in case of previous load)
	// TODO: put this in a better place
	characters.clear();
	characterButtons.clear();
	selectedCharacterButtons.clear();
	characterButtonBackground.unload();
	selectedCharacterButtonBackground.unload();
	goButton.lock();
	done = false;

	// load button backgrounds
	// character button backgrounds
	{
		std::istringstream textStream(loadInfo.characterButtonMenuText);
		characterButtonBackground.load(textStream);
	}
	{
		std::istringstream textStream(std::to_string(loadInfo.numberOfOutputCharacters * 32 + 8.0f) + "\n" + loadInfo.selectedCharacterButtonMenuText);
		selectedCharacterButtonBackground.load(textStream);
	}
	// character info
	{
		std::istringstream textStream(loadInfo.characterInfoMenuText);
		characterInfoDisplay.load(textStream);
	}

	// button info
	{
		std::istringstream textStream(loadInfo.buttonText);
		leftButton.load(textStream);
		leftButton.setText("<");
	}
	{
		std::istringstream textStream(loadInfo.buttonText);
		rightButton.load(textStream);
		rightButton.setText(">");
	}
	// exit Button
	{
		std::istringstream textStream(loadInfo.exitButtonText);
		exitButton.load(textStream);
	}
	// go Button
	{
		std::istringstream textStream(loadInfo.goButtonText);
		std::cout << "goButton text: " << loadInfo.goButtonText << std::endl;
		goButton.load(textStream);
	}



	// load selected character container
	selectedCharacterButtons.reserve(loadInfo.numberOfOutputCharacters);
	for (int i = 0; i < loadInfo.numberOfOutputCharacters; ++i) {
		selectedCharacterButtons.push_back(CharacterButton());
	} 
	// load all selected character buttons
	for (auto it = selectedCharacterButtons.begin(); it != selectedCharacterButtons.end(); ++it) {
		it->load();
		selectedCharacterButtonBackground.addCharacter(it);
	}



	// character and character button stuff

	characters.reserve(loadInfo.numberOfCharacters);
	characterButtons.reserve(loadInfo.numberOfCharacters);
	// set the character
	std::cout << "preparing to add " << loadInfo.numberOfCharacters << " characters." << std::endl;
	for (int id : loadInfo.characterIds) {
		std::cout << "new character of id: " << id << std::endl;
		characters.push_back(Character(id));
	}
	// load all characters, and add all character buttons
	for (auto it = characters.begin(); it != characters.end(); ++it) {
		it->load();
		characterButtons.push_back(CharacterButton(it));
	}
	// load all character buttons
	for (auto it = characterButtons.begin(); it != characterButtons.end(); ++it) {
		it->load();
		characterButtonBackground.addCharacter(it);
	}


	lastDisplayedCharacterButton = characterButtons.begin();
	latestSelectedCharacter = selectedCharacterButtons.begin();
	characterInfoDisplay.setCharacterData(characterButtons.begin()->characterRef);

	MenuItem::loadFileData(loadInfo.oldInfo);

	return true;
}




















void CharacterSelect::draw(sf::RenderWindow & window, sf::Time elapsedTime)
{
	drawBackground(window, boundingBox);

	// draw  options to choose from
	leftButton.draw(window, boundingBox);
	rightButton.draw(window, boundingBox);
	characterButtonBackground.draw(window, boundingBox, elapsedTime);

	// draw output box
	selectedCharacterButtonBackground.draw(window, boundingBox, elapsedTime);

	// draw selected character
	characterInfoDisplay.draw(window, boundingBox);
	lastDisplayedCharacterButton->characterRef->draw(window, boundingBox, elapsedTime);
	if (lastDisplayedCharacterButton->characterRef->isAnimationFinished()) {
		lastDisplayedCharacterButton->characterRef->startSecondaryAnimation(lastDisplayedCharacterButton->characterRef);
	}


	// draw exit button
	exitButton.draw(window, boundingBox);

	// draw go button
	goButton.draw(window, boundingBox);

	drawBorder(window, boundingBox);
}









void CharacterSelect::checkMouseDown(sf::Vector2f pos)
{
	leftButton.checkMouseDown(pos);
	rightButton.checkMouseDown(pos);
	exitButton.checkMouseDown(pos);

	// check character buttons
	for (auto it = characterButtons.begin(); it != characterButtons.end(); ++it) {
		it->attemptSelect(pos);
	}
	for (auto it = selectedCharacterButtons.begin(); it != selectedCharacterButtons.end(); ++it) {
		it->attemptSelect(pos);
	}

	// check go button
	goButton.checkMouseDown(pos);

}

void CharacterSelect::checkMouseUp(sf::Vector2f pos)
{
	int actionArg, actionType;
	// check exit button
	std::tie(actionType, actionArg) = exitButton.checkMouseUp(pos);
	if (actionType != -1) {
		done = true;
		return;
	}
	std::tie(actionArg, actionType) = leftButton.checkMouseUp(pos);
	if (actionArg == 0) {
		characterButtonBackground.scrollCharacter(-5);
		return;
	}
	std::tie(actionArg, actionType) = rightButton.checkMouseUp(pos);
	if (actionArg == 0) {
		characterButtonBackground.scrollCharacter(5);
		return;
	}


	// check character buttons
	for (auto it = characterButtons.begin(); it != characterButtons.end(); ++it) {
		if (it->checkSelected(pos)) {
			// add to the list
			selectCharacter(it);
		}
	}
	for (auto it = selectedCharacterButtons.begin(); it != selectedCharacterButtons.end(); ++it) {
		if (it->checkSelected(pos)) {
			// add to the list
			unselectCharacter(it);
		}
	}


	// check go button
	std::tie(actionType, actionArg) = goButton.checkMouseUp(pos);
	if (actionType != -1) {
		done = true;
		saveCharacters();
		return;
	}
}

void CharacterSelect::checkMouseMove(sf::Vector2f pos)
{
	for (auto it = characterButtons.begin(); it != characterButtons.end(); ++it) {
		if (it->contains(pos)) {
			displayCharacterInfo(it);
		}
	}
	characterInfoDisplay.checkMouseMove(pos);
}







void CharacterSelect::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
	updateItemPos();
}


void CharacterSelect::updateItemPos()
{
	float x, y;

	// place exit button
	exitButton.updatePos(sf::Vector2f(boundingBox.left + 8.0f, boundingBox.top + 8.0f));

	// update goButton pos
	x = boundingBox.left + (boundingBox.width - goButton.getWidth()) / 2.0f;
	y = boundingBox.top + boundingBox.height - 8.0f - goButton.getHeight();
	goButton.updatePos(sf::Vector2f(x, y));

	// update character info display position
	x = boundingBox.left + (boundingBox.width - characterInfoDisplay.getWidth()) / 2.0f;
	y -= characterInfoDisplay.getHeight();
	characterInfoDisplay.updatePos(sf::Vector2f(x, y));

	// update character buttons / navigation buttons
	x = boundingBox.left + (boundingBox.width - characterButtonBackground.getWidth()) / 2.0f;
	y -= characterButtonBackground.getHeight() + 8.0f;
	leftButton.updatePos(sf::Vector2f(x - leftButton.getWidth(), y));
	characterButtonBackground.updatePos(sf::Vector2f(x, y));
	rightButton.updatePos(sf::Vector2f(x + characterButtonBackground.getWidth(), y));

	// update output box
	x = boundingBox.left + (boundingBox.width - selectedCharacterButtonBackground.getWidth()) / 2.0f;
	y -= selectedCharacterButtonBackground.getHeight() + 4.0f;
	selectedCharacterButtonBackground.updatePos(sf::Vector2f(x, y));

	x = boundingBox.left + (boundingBox.width - 64.0f) / 2.0f;
	y -= 64.0f + 8;
	const sf::Vector2f characterPos = sf::Vector2f(x, y);
	for (auto it = characters.begin(); it != characters.end(); ++it) {
		it->updatePos(characterPos);
	}

}




void CharacterSelect::displayCharacterInfo(std::vector<CharacterButton>::iterator character)
{
	if (character == lastDisplayedCharacterButton) {
		// already displayed it, so don't bother
		return;
	}
	lastDisplayedCharacterButton = character;
	characterInfoDisplay.setCharacterData(character->characterRef);
}



void CharacterSelect::selectCharacter(std::vector<CharacterButton>::iterator character)
{
	std::cout << "in selectCharacter" << std::endl;
	std::cout << "check one is: " << (latestSelectedCharacter == selectedCharacterButtons.end()) << std::endl;

	// if the last one's there, then full capacity has been reached
	if (latestSelectedCharacter == selectedCharacterButtons.end()) {
		return;
	}
	std::cout << "passed check 1" << std::endl;

	// if the one we're trying to add is already in the list, then it's invalid
	for (auto it = selectedCharacterButtons.begin(); it != latestSelectedCharacter; ++it) {
		if (it->isCharacterDataSet() && it->characterRef == character->characterRef) {
			return;
		}
	}
	std::cout << "passed check 2" << std::endl;

	latestSelectedCharacter->setCharacterData(character->characterRef);
	if (++latestSelectedCharacter == selectedCharacterButtons.end()) {
		// all characters selected, unlock go button so people can play
		goButton.unlock();
	}
}

void CharacterSelect::unselectCharacter(std::vector<CharacterButton>::iterator character)
{// if the first one hasn't been set yet, there's nothing to remove
	if (character == latestSelectedCharacter) {
		return;
	}


	// if the latest one is before the selected, there's nothing to remove
	for (auto it = selectedCharacterButtons.begin(); it != character; ++it) {
		if (it == latestSelectedCharacter) {
			return;
		}
	}

	--latestSelectedCharacter;


	std::vector<CharacterButton>::iterator nextCharacter = character;
	for (auto it = character; it != latestSelectedCharacter; ++it) {
		it->setCharacterData((++nextCharacter)->characterRef);
	}
	latestSelectedCharacter->clearCharacterData();

	// a button has been removed, lock go button again
	goButton.lock();
	done = false;


	std::cout << "successfully unselected character!!" << std::endl;
}






void CharacterSelect::saveCharacters()
{
	std::fstream partyFile("gamestate/current.party", std::ios::out);
	if (partyFile.is_open()) {
		partyFile << selectedCharacterButtons.size() << std::endl;
		for (auto it = selectedCharacterButtons.begin(); it != selectedCharacterButtons.end(); ++it) {
			partyFile << std::to_string(it->characterType) << std::endl;
		}
	}
}





bool CharacterSelect::isDone()
{
	return done;
}
