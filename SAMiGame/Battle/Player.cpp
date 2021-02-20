#include "Player.h"

Player::Player()
{
}

Player::Player(float h)
{
	maxHealth = h;
	setHealth(h);
}

Player::~Player()
{
}

float Player::getHealth()
{
	return health;
}

float Player::getMaxHealth()
{
	return maxHealth;
}

void Player::setHealth(float h)
{
	health = h;
}

void Player::damage(float damage)
{
	if (dead) {
		return;
	}
	health -= damage;
	if (health <= 0.0f) {
		std::cout << "dead!" << std::endl;
		dead = true;
	}

	std::cout << "taking " << damage << " damage, health is now at: " << health << std::endl;
}


// TODO: add these
void Player::addEffects()
{
	if (dead) {
		return;
	}
}

void Player::calculateEffectDamage()
{
	if (dead) {
		return;
	}
}
