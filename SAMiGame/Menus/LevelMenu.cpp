#include "LevelMenu.h"

LevelMenu::LevelMenu()
{
	LevelMenu(0.0f, 0.0f);
}

LevelMenu::LevelMenu(float x, float y)
{
	boundingBox.left = x;
	boundingBox.top = y;

	state = loading;
}

LevelMenu::~LevelMenu()
{
}













// Loading

bool LevelMenu::load(int id)
{

	std::fstream file;
	file.open("data/levels/" + std::to_string(id) + "/.level", std::ios::in);
	if (!file.is_open()) 
	{
		std::cout << "error opening file: data / levels" + std::to_string(id) + ".level" << std::endl;
		return false;
	}

	levelid = id;

	// TODO: figure out how to integrate this with superclass
	LevelMenu::LoadInfo loadInfo;

	// load main text file
	std::string line;
	int i = 0;
	while (getline(file, line)) {
		if (line.substr(0, 2) == "//") {
			continue;
		}
		getFileLineData(++i, line, loadInfo);
	}


	// load player party data
	std::cout << "attempting to load party data" << std::endl;
	std::fstream partyFile("gamestate/current.party", std::ios::in);
	if (partyFile.is_open()) {
		std::cout << "opened file for part characters!" << std::endl;
		std::string line;
		int i = 0;
		while (getline(partyFile, line)) {
			if (line.substr(0, 2) == "//") {
				continue;
			}
			loadCharacters(++i, line, loadInfo.playerCharacterButtons);
		}
	}
	else {
		std::cout << "Failed to open file." << std::endl;
		// error?
	}

	// do stuff with the file data
	loadFileData(loadInfo);

	// now, load everything else
	loadTextureData();

	// at first, just display the first of the characters
	if (!playerCharacterButtons.empty()) {
		displayCharacterInfo(playerCharacterButtons.begin(), enemyCharacterButtons);
	}

	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
	updateItemPos();

	state = selecting;

	return true;
}

void LevelMenu::getFileLineData(int i, std::string & line, LoadInfo & loadInfo)
{
	if (i < 6) { // info for the background
		MenuItem::getFileLineData(i, line, loadInfo.oldInfo);
		return;
	}
	i -= 5;

	if (i < 6) {
		loadInfo.characterButtonMenuText.append(line + "\n");
		return;
	}
	i -= 5;

	if (i < 18) {
		loadInfo.characterInfoMenuText.append(line + "\n");
		return;
	}
	i -= 17;

	if (i < 10)
	{
		loadInfo.goButtonText.append(line + "\n");
		return;
	}
	i -= 9;

	loadCharacters(i, line, loadInfo.enemyCharacterButtons);
}

void LevelMenu::loadCharacters(int i, std::string & line, CharacterInfo & characterInfo)
{
	switch (i) {
	case 1:
	{
		characterInfo.numberOfCharacters = std::stoi(line);
		characterInfo.totalNumberOfCharacters = characterInfo.numberOfCharacters;
		std::cout << characterInfo.numberOfCharacters << " characters to be made!" << std::endl;
		break;
	}
	default:
	{
		if (characterInfo.numberOfCharacters > 0) {
			characterInfo.characterIds.push_back(std::stoi(line));
			--characterInfo.numberOfCharacters;
			std::cout << "Recieved data for a character. " << characterInfo.numberOfCharacters << " left" << std::endl;
		}
		break;
	}
	}
}


void LevelMenu::loadFileData(LoadInfo & loadInfo)
{


	if (loadInfo.playerCharacterButtons.totalNumberOfCharacters != loadInfo.enemyCharacterButtons.totalNumberOfCharacters) {
		// real bad news. Error.
		// TODO: figure out how to handle this. For now, just returns.
		return;
	}


	// character button backgrounds
	{
		std::istringstream textStream(loadInfo.characterButtonMenuText);
		playerCharacterButtonBackground.load(textStream);
	}
	{
		std::istringstream textStream(loadInfo.characterButtonMenuText);
		enemyCharacterButtonBackground.load(textStream);
	}
	{
		std::istringstream textStream(loadInfo.characterButtonMenuText);
		selectedPlayerCharacterButtonBackground.load(textStream);
	}

	// character info background
	{
		std::istringstream textStream(loadInfo.characterInfoMenuText);
		characterInfoDisplay.load(textStream);
	}

	// go Button
	{
		std::istringstream textStream(loadInfo.goButtonText);
		goButton.load(textStream);
	}


	// player character buttons
	// allocate vector space
	playerCharacterButtons.reserve(loadInfo.playerCharacterButtons.totalNumberOfCharacters);
	playerCharacters.reserve(loadInfo.playerCharacterButtons.totalNumberOfCharacters);
	for (int i : loadInfo.playerCharacterButtons.characterIds)
	{
		playerCharacters.push_back(Character(i));
	}

	for (auto it = playerCharacters.begin(); it != playerCharacters.end(); ++it)
	{
		it->load();
	}


	for (auto it = playerCharacters.begin(); it != playerCharacters.end(); ++it)
	{
		playerCharacterButtons.push_back(CharacterButton(it));
	}

	for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it)
	{
		it->load();
	}


	// enemy character buttons
	// allocate vector space
	enemyCharacterButtons.reserve(loadInfo.enemyCharacterButtons.totalNumberOfCharacters);
	enemyCharacters.reserve(loadInfo.enemyCharacterButtons.totalNumberOfCharacters);
	for (int i : loadInfo.enemyCharacterButtons.characterIds)
	{
		enemyCharacters.push_back(Character(i));
	}

	for (auto it = enemyCharacters.begin(); it != enemyCharacters.end(); ++it)
	{
		it->load(levelid);
	}

	for (auto it = enemyCharacters.begin(); it != enemyCharacters.end(); ++it)
	{
		enemyCharacterButtons.push_back(CharacterButton(it));
	}

	for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it)
	{
		it->load();
	}


	// selected player character buttons
	// allocate vector space
	selectedPlayerCharacterButtons.reserve(loadInfo.playerCharacterButtons.totalNumberOfCharacters);
	for (int i = 0; i < loadInfo.playerCharacterButtons.totalNumberOfCharacters; ++i) {
		selectedPlayerCharacterButtons.push_back(CharacterButton());
	}
	for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it)
	{
		it->load();
	}
	latestSelectedPlayerCharacter = selectedPlayerCharacterButtons.begin();

	// selected enemy character buttons
	// allocate vector space
	selectedEnemyCharacterButtons.reserve(loadInfo.enemyCharacterButtons.totalNumberOfCharacters);
	for (int i = 0; i < loadInfo.enemyCharacterButtons.totalNumberOfCharacters; ++i) {
		selectedEnemyCharacterButtons.push_back(CharacterButton());
	}
	for (auto it = selectedEnemyCharacterButtons.begin(); it != selectedEnemyCharacterButtons.end(); ++it)
	{
		it->load();
	}
	latestSelectedEnemyCharacter = selectedEnemyCharacterButtons.begin();




	// load players
	// TODO: load in player health from file
	player = Player(1000.0f);
	enemy = Player(1000.0f);

	playerHealthBar.load();
	enemyHealthBar.load();
	playerHealthBar.setHealth(player);
	enemyHealthBar.setHealth(enemy);


	MenuItem::loadFileData(loadInfo.oldInfo);
}












// Position updates

void LevelMenu::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
}

void LevelMenu::updateItemPos()
{
	float x, y;

	// place go button
	goButton.updatePos(sf::Vector2f(boundingBox.left + boundingBox.width / 2.0f - goButton.getWidth() / 2.0f, boundingBox.top + 100.0f));


	// place characters
	y = boundingBox.top + boundingBox.height / 2 - 32.0f;
	x = boundingBox.left + 32.0f;
	for (auto it = playerCharacters.begin(); it != playerCharacters.end(); ++it)
	{
		it->updatePos(sf::Vector2f(x, y));
	}

	// TODO: make this dependant on character size, not fixed:
	x = boundingBox.left + boundingBox.width - 32.0f;
	for (auto it = enemyCharacters.begin(); it != enemyCharacters.end(); ++it)
	{
		it->updatePos(sf::Vector2f(x, y));
	}



	// place healthBars
	y = boundingBox.top + 32.0f;
	x = boundingBox.left + 8.0f;
	playerHealthBar.updatePos(sf::Vector2f(x, y));
	x = boundingBox.left + boundingBox.width - 8.0f - enemyHealthBar.getWidth();
	enemyHealthBar.updatePos(sf::Vector2f(x, y));



	// place character buttons

	y = boundingBox.top + boundingBox.height / 2 - 32.0f;

	// Player characters go at the left-middle section
	x = boundingBox.left + 32.0f;

	// place player character button background
	playerCharacterButtonBackground.updatePos(sf::Vector2f(x - 4.0f, y - 4.0f));

	for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
		it->updatePos(sf::Vector2f(x, y));
		std::cout << "character of type: " << it->characterType << " positioned at: " << x << std::endl;
		// move to the right, leaving space between characters
		x += 32.0f + 4.0f;
	}


	// Enemy characters go at the right-middle section
	x = boundingBox.left + boundingBox.width - 32.0f - 32.0f;

	for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it) {
		it->updatePos(sf::Vector2f(x, y));
		std::cout << "character positioned at: " << x << std::endl;
		// move to the left, leaving space between characters
		x -= 32.0f + 4.0f;
	}

	// place enemy character button background
	enemyCharacterButtonBackground.updatePos(sf::Vector2f(x + 28.0f, y - 4.0f));




	// selected player character buttons go at the top-middle section
	x = boundingBox.left + boundingBox.width / 2.0f - selectedPlayerCharacterButtonBackground.getWidth() / 2.0f + 4.0f;
	y = 64.0f;

	// place player character button background
	selectedPlayerCharacterButtonBackground.updatePos(sf::Vector2f(x - 4.0f, y - 4.0f));

	for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it) {
		it->updatePos(sf::Vector2f(x, y));
		std::cout << "character of type: " << it->characterType << " positioned at: " << x << std::endl;
		// move to the right, leaving space between characters
		x += 32.0f + 4.0f;
	}

	// place character matchup info section
	characterInfoDisplay.updatePos(sf::Vector2f(boundingBox.left + 32.0f, boundingBox.top + boundingBox.height / 2 + 32));
}













// Drawing

void LevelMenu::draw(sf::RenderWindow & window, sf::Time elapsedTime)
{
	drawBackground(window, boundingBox);

	switch(state)
	{
	case selecting:
	{
		// draw character button backgrounds
		playerCharacterButtonBackground.draw(window, boundingBox);
		enemyCharacterButtonBackground.draw(window, boundingBox);
		selectedPlayerCharacterButtonBackground.draw(window, boundingBox);


		// draw player character buttons
		for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
			it->draw(window, boundingBox);
			//std::cout << "in levelmenu::draw, charactertype is: " << it->characterType << std::endl;
		}

		// draw enemy character buttons
		for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it) {
			it->draw(window, boundingBox);
		}

		// draw selected character buttons
		for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it) {
			it->draw(window, boundingBox);
		}

		// draw character matchup info
		characterInfoDisplay.draw(window, boundingBox);

		// draw go button
		goButton.draw(window, boundingBox);

		break;
	}
	case animating:
	{

		auto pCharacter = latestSelectedPlayerCharacter->characterRef;
		auto eCharacter = latestSelectedEnemyCharacter->characterRef;

		switch (animationState)
		{
		case intro:
		{


			animationTime += elapsedTime;
			if (animationTime > animationStartDelay) {
				animationTime = sf::milliseconds(0);

				pCharacter->startSecondaryAnimation(latestSelectedEnemyCharacter->characterRef);
				eCharacter->startSecondaryAnimation(latestSelectedPlayerCharacter->characterRef);


				animationState = secondaryAttack;
			}
			break;
		}
		case secondaryAttack:
		{

			if (pCharacter->isAnimationFinished() && eCharacter->isAnimationFinished()) {
				animationState = addEffects;
			}
			break;
		}
		case addEffects:
		{

			// TODO: add effects to player
			// pCharacter->addEffectsToPlayer(player);
			// pCharacter->addEffectsToOpponent(enemy);
			// eCharacter->addEffectsToPlayer(enemy);
			// eCharacter->addEffectsToOpponent(player);

			animationTime += elapsedTime;
			if (animationTime > animationPauseDelay) {
				animationTime = sf::milliseconds(0);

				pCharacter->startPrimaryAnimation(eCharacter);
				eCharacter->startPrimaryAnimation(pCharacter);
				animationState = primaryAttack;
			}
			break;
		}
		case primaryAttack:
		{
			if (pCharacter->isAnimationFinished() && eCharacter->isAnimationFinished()) {
				// TODO: update health graphics/logic
				player.damage(eCharacter->calculateDamage(pCharacter));
				enemy.damage(pCharacter->calculateDamage(eCharacter));

				playerHealthBar.setHealth(player);
				enemyHealthBar.setHealth(enemy);

				animationState = healthUpdate;
			}
			break;
		}
		case healthUpdate:
		{

			if (playerHealthBar.isAnimationFinished() && enemyHealthBar.isAnimationFinished())
			{
				animationState = endingPause;
			}

			break;
		}
		case endingPause:
		{
			animationTime += elapsedTime;
			if (animationTime > animationPauseDelay) {
				animationTime = sf::milliseconds(0);
				animationState = finishedCharacter;
			}
			break;
		}
		case finishedCharacter:
		{

			pCharacter->resetAnimation();
			eCharacter->resetAnimation();

			++latestSelectedPlayerCharacter;
			++latestSelectedEnemyCharacter;

			animationState = intro;

			if (latestSelectedPlayerCharacter == selectedPlayerCharacterButtons.end() || latestSelectedEnemyCharacter == selectedEnemyCharacterButtons.end())
			{
				// reset all player-selected characters
				while (latestSelectedPlayerCharacter != selectedPlayerCharacterButtons.begin())
				{
					unselectCharacter(selectedPlayerCharacterButtons.begin());
				}
				latestSelectedEnemyCharacter = selectedEnemyCharacterButtons.begin();
				state = selecting;
			}
			break;
		}
		}


		pCharacter->draw(window, boundingBox, elapsedTime);
		eCharacter->draw(window, boundingBox, elapsedTime);

		break;
	}
	}

	// draw healthbars
	playerHealthBar.draw(window, boundingBox, elapsedTime);
	enemyHealthBar.draw(window, boundingBox, elapsedTime);

	drawBorder(window, boundingBox);
}













// Mouse/Button checks

void LevelMenu::checkMouseDown(sf::Vector2f pos)
{
	switch (state)
	{
	case selecting:
	{
		// update all buttons for mouse click down

		// check player character buttons
		for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
			it->attemptSelect(pos);
		}

		// check enemy character buttons
		for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it) {
			it->attemptSelect(pos);
		}

		goButton.checkMouseDown(pos);
	}
	}
}

void LevelMenu::checkMouseUp(sf::Vector2f pos)
{
	switch (state)
	{
	case selecting:
	{
		// check player character buttons
		for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
			if (it->checkSelected(pos))
			{
				selectCharacter(it);
				return;
			}
		}

		// check enemy character buttons
		for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it) {
			if (it->checkSelected(pos))
			{
				unselectCharacter(it);
				return;
			}
		}

		int actionType, actionArg;
		std::tie(actionType, actionArg) = goButton.checkMouseUp(pos);
		if (actionType != -1) {
			std::cout << "GO GO GO!" << std::endl;
			startAnimationState();
			return;
		}
	}
	}
}

void LevelMenu::checkMouseMove(sf::Vector2f pos)
{

	switch (state)
	{
	case selecting:
	{
		// update all buttons for  mouse click up

		// check player character buttons
		for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it) {
			if (it->contains(pos)) {
				//std::cout << "player character button clicked of type: " << it->characterType << std::endl;
				//std::cout << "name: " << getCharacterTemplate((it->characterType))->name << std::endl;
				displayCharacterInfo(it, enemyCharacterButtons);
				return;
			}
		}

		/*// check player character buttons
		for (auto it = selectedPlayerCharacterButtons.begin(); it != selectedPlayerCharacterButtons.end(); ++it) {
			if (it->contains(pos)) {
				std::cout << "player character button clicked of type: " << it->characterType << std::endl;
				//std::cout << "name: " << getCharacterTemplate((it->characterType))->name << std::endl;
				displayCharacterInfo(it, enemyCharacterButtons);
				return;
			}
		}*/

		// check enemy character buttons
		for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it) {
			if (it->contains(pos)) {
				//std::cout << "enemy character button clicked of type: " << it->characterType << std::endl;
				//std::cout << "name: " << getCharacterTemplate((it->characterType))->name << std::endl;
				displayCharacterInfo(it, playerCharacterButtons);
				return;
			}
		}
	}
	}
}













// Character selection management

void LevelMenu::displayCharacterInfo(std::vector<CharacterButton>::iterator character, std::vector<CharacterButton> & otherlist)
{
	characterInfoDisplay.setCharacterData(character->characterRef);

	// clear both player and enemy matchups
	for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it)
	{
		it->setMatchup(0, 0);
	}
	for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it)
	{
		it->setMatchup(0, 0);
	}


	// set the matchups
	for (auto it = otherlist.begin(); it != otherlist.end(); ++it)
	{
		it->setMatchup((int)(character->characterRef->getPrimaryMatchup(it->characterRef)),
					   (int)(2* character->characterRef->getSecondaryMatchup(it->characterRef)));
	}
}

void LevelMenu::selectCharacter(std::vector<CharacterButton>::iterator character)
{

	std::cout << "in selectCharacter" << std::endl;
	std::cout << "check one is: " << (latestSelectedPlayerCharacter == selectedPlayerCharacterButtons.end()) << std::endl;

	// if the last one's there, then full capacity has been reached
	if (latestSelectedPlayerCharacter == selectedPlayerCharacterButtons.end()) {
		return;
	}
	std::cout << "passed check 1" << std::endl;

	// if the one we're trying to add is already in the list, then it's invalid
	for (auto it = selectedPlayerCharacterButtons.begin(); it != latestSelectedPlayerCharacter; ++it) {
		if (it->isCharacterDataSet() && it->characterRef == character->characterRef) {
			return;
		}
	}
	std::cout << "passed check 2" << std::endl;

	latestSelectedPlayerCharacter->setCharacterData(character->characterRef);
	if (++latestSelectedPlayerCharacter == selectedPlayerCharacterButtons.end()) {
		// all characters selected, unlock go button so people can play
		goButton.unlock();
	}

	




}

void LevelMenu::unselectCharacter(std::vector<CharacterButton>::iterator character)
{

	// if the first one hasn't been set yet, there's nothing to remove
	if (character == latestSelectedPlayerCharacter) {
		return;
	}


	// if the latest one is before the selected, there's nothing to remove
	for (auto it = selectedPlayerCharacterButtons.begin(); it != character; ++it) {
		if (it == latestSelectedPlayerCharacter) {
			return;
		}
	}

	--latestSelectedPlayerCharacter;


	std::vector<CharacterButton>::iterator nextCharacter = character;
	for (auto it = character; it != latestSelectedPlayerCharacter; ++it) {
		it->setCharacterData((++nextCharacter)->characterRef);
	}
	latestSelectedPlayerCharacter->clearCharacterData();

	// a button has been removed, lock go button again
	goButton.lock();


	std::cout << "successfully unselected character!!" << std::endl;

}













// Animation manager

void LevelMenu::startAnimationState()
{

	chooseEnemyCharacters();

	
	latestSelectedPlayerCharacter = selectedPlayerCharacterButtons.begin();
	latestSelectedEnemyCharacter  = selectedEnemyCharacterButtons.begin();

	animationTime = sf::milliseconds(0);

	state = animating;
	animationState = intro;
}

void LevelMenu::chooseEnemyCharacters()
{
	latestSelectedEnemyCharacter = selectedEnemyCharacterButtons.begin();
	for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it)
	{
		latestSelectedEnemyCharacter->setCharacterData(it->characterRef);
		++latestSelectedEnemyCharacter;
	}
}
