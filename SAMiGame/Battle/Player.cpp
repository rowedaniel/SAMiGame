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

void Player::flatDamage(float damage)
{
	health -= damage;
	if (health <= 0.0f) {
		std::cout << "dead!" << std::endl;
		dead = true;
	}
}

void Player::damage(float damage)
{
	if (dead) {
		return;
	}

	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		damage = (*it->applyToDefense)(*it, damage, this);
	}

	flatDamage(damage);

	std::cout << "taking " << damage << " damage, health is now at: " << health << std::endl;
}




void Player::addEffect(Effect effect)
{
	if (dead) {
		return;
	}
	activeEffects.push_back(effect);
}
bool Player::isDead()
{
	return dead;
}
void Player::beforeRoundEffects()
{
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		(*it->beforeRound)(*it, this);
	}
}
void Player::afterRoundEffects()
{
	for (auto it = activeEffects.begin(); it != activeEffects.end(); ++it) {
		(*it->afterRound)(*it, this);
	}
}
void Player::incrementEffectTimer()
{
	auto it = activeEffects.begin();
	while (it != activeEffects.end()) {
		it->advanceTurn();
		if (it->isExpired())
		{
			std::cout << "effect expired :(" << std::endl;
			it = activeEffects.erase(it);
			continue;
		}
		++it;
	}
}