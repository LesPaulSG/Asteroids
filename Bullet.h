#pragma once
#include "Wall.h"

class Bullet {
private:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	float time;
	float lifeTime;
	bool alive;
	sf::RectangleShape body;

public:
	const sf::RectangleShape& GetBody() const;
	bool GetAlive() const;

	Bullet(sf::Vector2f pos, float dir, float speed, float lifeTime);
	~Bullet() = default;

	void CheckCollision(std::vector<Wall>& walls, const sf::Vector2f& oldPos);
	void Update(float t, std::vector<Wall>& walls);
	void LifeCheck();
};