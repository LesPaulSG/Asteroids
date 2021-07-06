#pragma once
#include "Polygon.h"

class Asteroid{
private:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	float radius;
	int stage;
	Polygon body;
	bool alive;
public:
	Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_ = 3);
	
	void Move(float time, std::vector<Asteroid>& asteroids);
	void CheckCollision(std::vector<Asteroid>& asteroids);
	void UpdateDirection(sf::Vector2f newDir);
	void Draw(sf::RenderWindow& w);
	void Destroy();

	const sf::Vector2f& GetPos() const;
	float GetRadius() const;
	bool GetAlive() const;
	int GetStage() const;

	bool isCollision(Line line);
};

