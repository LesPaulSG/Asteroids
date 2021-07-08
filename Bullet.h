#pragma once
#include "Asteroid.h"

class Bullet {
private:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	float time;
	float lifeTime;
	bool alive;
	sf::CircleShape body;

public:
	const sf::CircleShape& GetBody() const;
	bool isAlive() const;

	Bullet(Shot sho);
	~Bullet() = default;

	void CheckCollision(std::vector<Actor*>& actors, const sf::Vector2f& oldPos);
	void Update(float t, std::vector<Actor*>& actors);
	void PushForward(float distance);
	void LifeCheck();
};