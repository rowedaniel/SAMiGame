#pragma once
#include "../stdafx.h"
#include "CharacterTemplate.h"
#include "EffectGetter.h"
#include "Effects/Effect.h"
#include "../Menus/EffectButton.h"
#include "Player.h"


class Character : public CharacterTemplate
{
public:
	Character(int characterType);
	~Character();

	void load(); // assume owned by player
	void load(int levelnumber); // owned by level
	void load(std::string path);
	void draw(sf::RenderWindow & window, sf::FloatRect boundBox, sf::Time elapsedTime);
	void updatePos(sf::Vector2f pos);

	sf::FloatRect getBounds();


	// animation controllers
	void resetAnimation();
	void startPrimaryAnimation(std::vector<Character>::iterator opponent);
	void startSecondaryAnimation(std::vector<Character>::iterator opponent);
	bool isAnimationFinished();

	// battle logic
	float getPrimaryMatchup(std::vector<Character>::iterator opponent);
	float getSecondaryMatchup(std::vector<Character>::iterator opponent);
	// gets raw attack damage
	float getAttack();
	// calculates damage against opponent, taking effects and type matchup into account
	float calculateDamage(std::vector<Character>::iterator opponent);
	
	// gets secondary effect cooldown
	int getEffectCooldown();
	void increaseEffectCooldown(int duration);

	// calculate secondary effect amounts
	EffectGetter::EffectInfo calculateSecondaryAmount(std::vector<Character>::iterator opponent, EffectGetter::EffectInfo & inputInfo);

	// applies secondary effects to self, opponent, player, and opponent character
	void applySecondary(std::vector<Character>::iterator opponent, Player & player, Player & opponentPlayer);
	
	// adds a new effect to this character.
	void addEffect(Effect effect);
	void incrementEffectTimer();

	void beforeAttackEffects();
	void afterAttackEffects();

	std::list<Effect> getEffects();
	bool isPlayerOwned();

	// for displaying active effects
	void checkMouseMove(sf::Vector2f pos);

	void updateXp(std::vector<Character> & opponents);
	int getLevel();

private:

	bool playerOwned = false; // determines if this character is owned by the player, or a CPU

	// loading stuff
	bool loaded = false;
	struct LoadInfo
	{
		unsigned int level = 0;
		float xp = 0.0f;
	};


	const std::vector<float> primaryMatchups = {0.0f, 2.0f, 1.0f, 1.0f};
	const std::vector<float> secondaryMatchups = { 1.0f, 2.0f, 1.5f, 0.5f, 0.0f }; 



	void getFileLineData(int i, std::string & line, LoadInfo & loadInfo);
	void loadFileData(LoadInfo & loadInfo);
	void loadTextureData();

	void updateSpritePos();
	void updateItemPos();




	// animation stuff
	sf::IntRect animationRect = sf::IntRect(0, 64*3, 64, 64);
	sf::Sprite sprite;

	sf::Vector2f position;

	sf::Time animationTime = sf::milliseconds(0);
	const sf::Time timestep = sf::milliseconds(55);
	bool animating = false;
	
	// used to move sprite around when animating
	float animationOffset = 0.0f;


	


	// character stat stuff
	float levelFactor;
	std::list<Effect> activeEffects;
	std::list<EffectButton> activeEffectButtons;

	int cooldownTimer = 0;

	int level;
	float xp;


};