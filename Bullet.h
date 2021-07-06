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
	bool GetAlive() const;

	Bullet(sf::Vector2f pos, float dir, float speed, float lifeTime);
	~Bullet() = default;

	void CheckCollision(std::vector<Asteroid>& asteroids, const sf::Vector2f& oldPos);
	void Update(float t, std::vector<Asteroid>& asteroids);
	void LifeCheck();
};