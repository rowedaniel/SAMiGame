#pragma once
#include "../stdafx.h"
#include "MenuItem.h"
#include "../Battle/Character.h"
#include "CharacterButton.h"
#include "CharacterButtonMenu.h"
#include "CharacterInfoDisplay.h"
#include "MenuButton.h"
#include "LockableMenuButton.h"


class CharacterSelect : public MenuItem
{
public:
	CharacterSelect();
	~CharacterSelect();


	bool load(int numberOfCharacters);
	void unload();

	void draw(sf::RenderWindow & window, sf::Time elapsedTime);

	void checkMouseDown(sf::Vector2f pos);
	void checkMouseUp(sf::Vector2f pos);
	void checkMouseMove(sf::Vector2f pos);

	void updatePos(sf::Vector2f pos);

	bool isDone();


private:
	struct LoadInfo {
		MenuItem::LoadInfo oldInfo;
		std::string exitButtonText;
		std::string characterButtonMenuText;
		std::string selectedCharacterButtonMenuText;
		std::string characterInfoMenuText;
		std::string buttonText;
		std::string goButtonText;
		unsigned int numberOfCharacters = 0;
		unsigned int numberOfOutputCharacters = 0;
		std::list<unsigned int> characterIds;
	};

	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	bool loadFileData(LoadInfo & loadInfo);

	void updateItemPos();

	void displayCharacterInfo(std::vector<CharacterButton>::iterator character);


	void selectCharacter(std::vector<CharacterButton>::iterator character);
	void unselectCharacter(std::vector<CharacterButton>::iterator character);

	void saveCharacters();

	
	std::vector<Character> characters;

	std::vector<CharacterButton> characterButtons;
	CharacterButtonMenu characterButtonBackground;
	std::vector<CharacterButton>::iterator lastDisplayedCharacterButton;

	std::vector<CharacterButton> selectedCharacterButtons;
	CharacterButtonMenu selectedCharacterButtonBackground;
	std::vector<CharacterButton>::iterator latestSelectedCharacter;

	MenuButton leftButton;
	MenuButton rightButton;

	CharacterInfoDisplay characterInfoDisplay;

	LockableMenuButton goButton;
	MenuButton exitButton;

	bool done = false;
};