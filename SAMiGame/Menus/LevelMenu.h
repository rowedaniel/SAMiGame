#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "CharacterButton.h"
#include "CharacterInfoDisplay.h"
#include "LockableMenuButton.h"
#include "../Battle/Character.h"

class LevelMenu : public MenuItem
{
public:
	LevelMenu();
	LevelMenu(float x, float y);
	~LevelMenu();
	
	void load(std::istream& file);
	void draw(sf::RenderWindow & window);
	
	void checkMouseDown(sf::Vector2f pos);
	void checkMouseUp(sf::Vector2f pos);
	void checkMouseMove(sf::Vector2f pos);
	void updatePos(sf::Vector2f pos);
	void updateItemPos();

private:

	enum BattleState { loading, selecting, animating };
	BattleState state;


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
		CharacterInfo playerCharacterButtons;
		CharacterInfo enemyCharacterButtons;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadCharacters(int i, std::string & line, CharacterInfo & characterInfo);
	void loadFileData(LoadInfo & loadInfo);
	// display matchups between character and list of other characters
	void displayCharacterInfo(std::vector<CharacterButton>::iterator character, std::vector<CharacterButton> & otherlist);

	void selectCharacter(std::vector<CharacterButton>::iterator character);
	void unselectCharacter(std::vector<CharacterButton>::iterator character);


	void startAnimationState();


	// character select stuff

	MenuItem enemyCharacterButtonBackground;
	MenuItem playerCharacterButtonBackground;
	MenuItem selectedPlayerCharacterButtonBackground;
	std::vector<CharacterButton> enemyCharacterButtons;
	std::vector<CharacterButton> playerCharacterButtons;
	std::vector<CharacterButton> selectedPlayerCharacterButtons;
	std::vector<CharacterButton>::iterator latestSelectedCharacter;

	std::vector<Character> enemyCharacters;
	std::vector<Character> playerCharacters;

	CharacterInfoDisplay characterInfoDisplay;
	LockableMenuButton goButton;
};