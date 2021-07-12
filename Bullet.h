#pragma once
#include "Asteroid.h"

class Bullet {
private:
	sf::CircleShape body;
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	float time;
	float lifeTime;
	bool alive;
	bool playerOwned;

public:
	const sf::CircleShape& GetBody() const;
	bool isAlive() const;
	bool isPlayerOwned() const;

	Bullet(Shot sho, bool playerShooted = false);
	~Bullet() = default;

	void CheckCollision(std::vector<Actor*>& actors, const sf::Vector2f& oldPos);
	void Update(float t, std::vector<Actor*>& actors);
	void PushForward(float distance);
	void LifeCheck();
};