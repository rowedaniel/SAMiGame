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
		std::cout << "error opening file: data/levels/" + std::to_string(id) + "/.level" << std::endl;
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
	if (!loadFileData(loadInfo)) {
		return false;
	}

	// now, load everything else
	loadTextureData();


	std::cout << "displaying character info for first character!" << std::endl;
	// at first, just display the first of the characters
	if (!playerCharacterButtons.empty()) {
		lastDisplayedCharacter = playerCharacterButtons.end();
		displayCharacterInfo(playerCharacterButtons.begin(), enemyCharacterButtons, true);
	}
	std::cout << "finished displaying character info" << std::endl;

	updatePos(sf::Vector2f(boundingBox.left, boundingBox.top));
	updateItemPos();

	state = selecting;
	done = false;
	won = false;



	std::cout << "finished load()" << std::endl;
	return true;
}

void LevelMenu::unload()
{
	// do something here?

}

void LevelMenu::getFileLineData(int i, std::string & line, LoadInfo & loadInfo)
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

	if (i < 5) {
		loadInfo.characterButtonMenuText.append(line + "\n");
		return;
	}
	i -= 4;

	if (i < 24) {
		loadInfo.characterInfoMenuText.append(line + "\n");
		return;
	}
	i -= 23;

	if (i < 10)
	{
		loadInfo.goButtonText.append(line + "\n");
		return;
	}
	i -= 9;

	if (i < 2) {
		loadInfo.winButtonId = std::stoi(line);
		return;
	}
	i -= 1;

	if (i < 2) {
		loadInfo.enemyHealth = std::stof(line);
		return;
	}
	i -= 1;

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


bool LevelMenu::loadFileData(LoadInfo & loadInfo)
{


	if (loadInfo.playerCharacterButtons.totalNumberOfCharacters != loadInfo.enemyCharacterButtons.totalNumberOfCharacters) {
		// mismatch between number of player characters and number of level characters.
		std::cout << "player/level character number mismatch: " << loadInfo.playerCharacterButtons.totalNumberOfCharacters << " and " << \
			loadInfo.enemyCharacterButtons.totalNumberOfCharacters << std::endl;
		return false;
	}

	// reset anything (in case of previous load)
	// TODO: put this in better place
	enemyCharacterButtons.clear();
	playerCharacterButtons.clear();
	selectedPlayerCharacterButtons.clear();
	selectedEnemyCharacterButtons.clear();
	playerCharacterButtonBackground.unload();
	enemyCharacterButtonBackground.unload();
	selectedPlayerCharacterButtonBackground.unload();
	enemyCharacters.clear();
	playerCharacters.clear();


	// character button backgrounds
	{
		std::istringstream textStream(std::to_string(loadInfo.playerCharacterButtons.totalNumberOfCharacters * 32 + 8) + "\n" + loadInfo.characterButtonMenuText);
		playerCharacterButtonBackground.load(textStream);
	}
	{
		std::istringstream textStream(std::to_string(loadInfo.playerCharacterButtons.totalNumberOfCharacters * 32 + 8) + "\n" + loadInfo.characterButtonMenuText);
		enemyCharacterButtonBackground.load(textStream);
	}
	{
		std::istringstream textStream(std::to_string(loadInfo.playerCharacterButtons.totalNumberOfCharacters * 32 + 8) + "\n" + loadInfo.characterButtonMenuText);
		selectedPlayerCharacterButtonBackground.load(textStream);
	}

	// character info
	{
		std::istringstream textStream(loadInfo.characterInfoMenuText);
		characterInfoDisplay.load(textStream);
	}


	// exit Button
	{
		std::istringstream textStream(loadInfo.exitButtonText);
		exitButton.load(textStream);
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
		// add to appropriate button backgroung menu
		playerCharacterButtonBackground.addCharacter(it);
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
		// add to appropriate button backgroung menu
		enemyCharacterButtonBackground.addCharacter(it);
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
		// add to appropriate button backgroung menu
		selectedPlayerCharacterButtonBackground.addCharacter(it);
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
	// TODO: put this in a more reasonable place
	{
		std::fstream file("gamestate/current.player", std::ios::in);
		if (file.is_open()) {


			std::string line;
			float playerMaxHealth = 0.0f;
			float playerHealth;

			// line 1
			while (getline(file, line)) {
				if (line.substr(0, 2) == "//") {
					continue;
				}
				playerMaxHealth = std::stof(line);
				break;
			}

			// by default, set current health to max health
			playerHealth = playerMaxHealth;

			// line 2
			while (getline(file, line)) {
				if (line.substr(0, 2) == "//") {
					continue;
				}
				playerHealth = std::stof(line);
				break;
			}
			player = Player(playerMaxHealth);
			player.setHealth(playerHealth);
		}
	}
	enemy = Player(loadInfo.enemyHealth);

	playerHealthBar.load();
	enemyHealthBar.load();
	playerHealthBar.update(player, true);
	enemyHealthBar.update(enemy, false);

	winButtonId = loadInfo.winButtonId;


	MenuItem::loadFileData(loadInfo.oldInfo);
	std::cout << "finished loading level info" << std::endl;

	// everything worked successfully
	return true;
}

void LevelMenu::onWin()
{
	won = true;
	{
		// first, check if the button is already unlocked:
		std::fstream unlockedButtonFile("gamestate/unlocked.buttons", std::ios::in);
		if (unlockedButtonFile.is_open()) {
			std::string line;
			int i = 0;
			while (getline(unlockedButtonFile, line)) {
				if (line == std::to_string(winButtonId)) {
					std::cout << "level button already unlocked, so skipping" << std::endl;
					return;
				}
			}
		}
	}
	{
		std::fstream unlockedButtonFile("gamestate/unlocked.buttons", std::ios::app);
		if (unlockedButtonFile.is_open()) {
			unlockedButtonFile << std::to_string(winButtonId) << std::endl;
		}
	}

}













// Position updates

void LevelMenu::updatePos(sf::Vector2f pos)
{
	MenuItem::updatePos(pos);
}

void LevelMenu::updateItemPos()
{
	float x, y;

	// place exit button
	exitButton.updatePos(sf::Vector2f(boundingBox.left + 8.0f, boundingBox.top + 8.0f));
	// place go button
	goButton.updatePos(sf::Vector2f(boundingBox.left + boundingBox.width / 2.0f - goButton.getWidth() / 2.0f, boundingBox.top + 100.0f));


	// place characters
	y = boundingBox.top + boundingBox.height / 2 - 64.0f;
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
	// player and enemy buttons go at middle height
	y = boundingBox.top + boundingBox.height / 2 - 32.0f -4.0f;

	// place player character button background
	// Player characters go at the left-middle section
	x = boundingBox.left + 32.0f + 4.0f;
	playerCharacterButtonBackground.updatePos(sf::Vector2f(x, y));

	// place enemy character button background
	// Enemy characters go at the right-middle section
	x = boundingBox.left + boundingBox.width - enemyCharacterButtonBackground.getWidth() - 32.0f - 4.0f;
	enemyCharacterButtonBackground.updatePos(sf::Vector2f(x, y));

	// place selected player character button background
	// selected player character buttons go at the top-middle section
	x = boundingBox.left + boundingBox.width / 2.0f - selectedPlayerCharacterButtonBackground.getWidth() / 2.0f + 4.0f;
	y = 64.0f;
	selectedPlayerCharacterButtonBackground.updatePos(sf::Vector2f(x - 4.0f, y - 4.0f));

	// place character matchup info section
	characterInfoDisplay.updatePos(sf::Vector2f(boundingBox.left + 40.0f, boundingBox.top + boundingBox.height / 2.0f + 32.0f));
}











bool LevelMenu::isDone()
{
	return done;
}

int LevelMenu::getButtonToUnlock()
{
	if (won) {
		return winButtonId;
	}
	return 0;
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
		playerCharacterButtonBackground.draw(window, boundingBox, elapsedTime);
		enemyCharacterButtonBackground.draw(window, boundingBox, elapsedTime);
		selectedPlayerCharacterButtonBackground.draw(window, boundingBox, elapsedTime);

		// draw character matchup info
		characterInfoDisplay.draw(window, boundingBox);

		// draw exit button
		exitButton.draw(window, boundingBox);

		// draw go button
		goButton.draw(window, boundingBox);

		break;
	}
	case animating:
	{

		// TODO: move this to an actual reasonable place

		auto pCharacter = latestSelectedPlayerCharacter->characterRef;
		auto eCharacter = latestSelectedEnemyCharacter->characterRef;

		switch (animationState)
		{
		case intro:
		{


			animationTime += elapsedTime;
			if (animationTime > animationStartDelay) {
				animationTime = sf::milliseconds(0);

				// TODO: probably place this in a better place
				pCharacter->beforeAttackEffects();
				eCharacter->beforeAttackEffects();

				pCharacter->startSecondaryAnimation(latestSelectedEnemyCharacter->characterRef);
				eCharacter->startSecondaryAnimation(latestSelectedPlayerCharacter->characterRef);

				animationState = secondaryAttack;
			}
			break;
		}
		case secondaryAttack:
		{

			if (pCharacter->isAnimationFinished() && eCharacter->isAnimationFinished()) {
				pCharacter->applySecondary(eCharacter, player, enemy);
				eCharacter->applySecondary(pCharacter, enemy, player);
				animationState = addEffects;

				playerHealthBar.update(player, true);
				enemyHealthBar.update(enemy, false);
			}
			break;
		}
		case addEffects:
		{

			animationTime += elapsedTime;
			if (animationTime > animationPauseDelay) {
				animationTime = sf::milliseconds(0);

				if (player.isDead()) {
					animationState = loss;
					break;
				}
				if (enemy.isDead()) {
					animationState = win;
					break;
				}

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

				playerHealthBar.update(player, true);
				enemyHealthBar.update(enemy, false);

				animationState = healthUpdate;
			}
			break;
		}
		case healthUpdate:
		{

			if (playerHealthBar.isAnimationFinished() && enemyHealthBar.isAnimationFinished())
			{
				if (player.isDead()) {
					animationState = loss;
					break;
				}
				if (enemy.isDead()) {
					animationState = win;
					break;
				}
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
				animationState = updatePlayerEffects;

				pCharacter->afterAttackEffects();
				eCharacter->afterAttackEffects();

				playerHealthBar.update(player, true);
				enemyHealthBar.update(enemy, false);


				while (latestSelectedPlayerCharacter != selectedPlayerCharacterButtons.begin())
				{
					unselectCharacter(selectedPlayerCharacterButtons.begin());
				}
				latestSelectedEnemyCharacter = selectedEnemyCharacterButtons.begin();
			}
			break;
		}
		case updatePlayerEffects:
		{

			if (playerHealthBar.isAnimationFinished() && enemyHealthBar.isAnimationFinished())
			{
				// round is over
				std::cout << "finished with 1 full round!" << std::endl;


				// update all the effect timers
				for (auto it = playerCharacterButtons.begin(); it != playerCharacterButtons.end(); ++it)
				{
					it->characterRef->incrementEffectTimer();
				}
				for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it)
				{
					it->characterRef->incrementEffectTimer();
				}

				player.afterRoundEffects();
				enemy.afterRoundEffects();

				playerHealthBar.update(player, true);
				enemyHealthBar.update(enemy, false);

				player.incrementEffectTimer();
				enemy.incrementEffectTimer();



				if (player.isDead()) {
					animationState = loss;
					break;
				}
				if (enemy.isDead()) {
					animationState = win;
					break;
				}

				// reset all player-selected characters
				animationState = intro;
				state = selecting;
			}
			break;
		}
		case win:
		{
			if (done) {
				break;
			}
			done = true;
			std::cout << "win!!!" << std::endl;
			onWin();
			break;
		}
		case loss:
		{
			if (done) {
				break;
			}
			done = true;
			won = false;
			std::cout << "loss :(" << std::endl;
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
		exitButton.checkMouseDown(pos);
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
		// check exit button
		std::tie(actionType, actionArg) = exitButton.checkMouseUp(pos);
		if (actionType != -1) {
			done = true;
			won = false;
			return;
		}
		// check go button
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
	// always update player health bar, so effects show up nicely
	playerHealthBar.checkMouseMove(pos);
	enemyHealthBar.checkMouseMove(pos);

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
				displayCharacterInfo(it, enemyCharacterButtons, true);
				return;
			}
		}

		// check enemy character buttons
		for (auto it = enemyCharacterButtons.begin(); it != enemyCharacterButtons.end(); ++it) {
			if (it->contains(pos)) {
				//std::cout << "enemy character button clicked of type: " << it->characterType << std::endl;
				//std::cout << "name: " << getCharacterTemplate((it->characterType))->name << std::endl;
				displayCharacterInfo(it, playerCharacterButtons, false);
				return;
			}
		}

		// update character info displayer
		characterInfoDisplay.checkMouseMove(pos);
		break;
	}
	case animating:
	{
		latestSelectedPlayerCharacter->characterRef->checkMouseMove(pos);
		latestSelectedEnemyCharacter->characterRef->checkMouseMove(pos);
		break;
	}
	}
}













// Character selection management

void LevelMenu::displayCharacterInfo(std::vector<CharacterButton>::iterator character, std::vector<CharacterButton> & otherlist, bool playerOwned)
{
	if (playerOwned == lastSelectedGroup && character == lastDisplayedCharacter) {
		return;
	}
	lastDisplayedCharacter = character;
	lastSelectedGroup = playerOwned;


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

	// before anything, trigger the player before round effects
	player.beforeRoundEffects();
	enemy.beforeRoundEffects();

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
