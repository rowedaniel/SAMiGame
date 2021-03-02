#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "CharacterButton.h"
#include "CharacterInfoDisplay.h"
#include "HealthBar.h"
#include "LockableMenuButton.h"
#include "../Battle/Character.h"
#include "../Battle/Player.h"

class LevelMenu : public MenuItem
{
public:
	LevelMenu();
	LevelMenu(float x, float y);
	~LevelMenu();
	
	bool load(int id);
	void unload();
	void draw(sf::RenderWindow & window, sf::Time elapsedTime);
	
	void checkMouseDown(sf::Vector2f pos);
	void checkMouseUp(sf::Vector2f pos);
	void checkMouseMove(sf::Vector2f pos);
	void updatePos(sf::Vector2f pos);
	void updateItemPos();

	bool isDone();
	int getButtonToUnlock();

private:

	enum BattleState { loading, selecting, animating };
	enum AnimationState { intro, secondaryAttack, addEffects, primaryAttack, healthUpdate, endingPause, finishedCharacter, updatePlayerEffects, loss, win };
	BattleState state;
	AnimationState animationState;


	struct CharacterInfo
	{
		int numberOfCharacters = 0; // this one is used while loading, decrements to 0
		int totalNumberOfCharacters = 0; // this one doesn't change, after set
		// 4, normally?
		std::list<int> characterIds;
	};

	struct LoadInfo
	{
	public:
		MenuItem::LoadInfo oldInfo;
		std::string characterButtonMenuText;
		std::string characterInfoMenuText;
		std::string goButtonText;
		int winButtonId = 0;
		float enemyHealth = 100.0f;
		CharacterInfo playerCharacterButtons;
		CharacterInfo enemyCharacterButtons;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadCharacters(int i, std::string & line, CharacterInfo & characterInfo);
	bool loadFileData(LoadInfo & loadInfo);
	void onWin();

	// display matchups between character and list of other characters
	void displayCharacterInfo(std::vector<CharacterButton>::iterator character, std::vector<CharacterButton> & otherlist, bool playerOwned);

	void selectCharacter(std::vector<CharacterButton>::iterator character);
	void unselectCharacter(std::vector<CharacterButton>::iterator character);


	void startAnimationState();
	void chooseEnemyCharacters();




	int levelid;

	// character select stuff

	MenuItem enemyCharacterButtonBackground;
	MenuItem playerCharacterButtonBackground;
	MenuItem selectedPlayerCharacterButtonBackground;
	std::vector<CharacterButton> enemyCharacterButtons;
	std::vector<CharacterButton> playerCharacterButtons;
	std::vector<CharacterButton> selectedPlayerCharacterButtons;
	std::vector<CharacterButton>::iterator latestSelectedPlayerCharacter;
	std::vector<CharacterButton> selectedEnemyCharacterButtons;
	std::vector<CharacterButton>::iterator latestSelectedEnemyCharacter;

	std::vector<Character> enemyCharacters;
	std::vector<Character> playerCharacters;

	CharacterInfoDisplay characterInfoDisplay;
	bool lastSelectedGroup = true;
	std::vector<CharacterButton>::iterator lastDisplayedCharacter;


	LockableMenuButton goButton;

	Player player;
	Player enemy;

	HealthBar playerHealthBar;
	HealthBar enemyHealthBar;






	// animation stuff
	sf::Time animationTime = sf::milliseconds(0);
	const sf::Time animationStartDelay = sf::milliseconds(500);
	const sf::Time animationPauseDelay = sf::milliseconds(500);

	bool done = false;
	bool won = false;

	int winButtonId;
};