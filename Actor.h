#pragma once
#include "Polygon.h"

class Actor {
private:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	float radius;
	Polygon body;
	
public:
	Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<sf::Vector2f>& points);

	void Move(float time, std::vector<Actor>& asteroids);
	bool Collision(std::vector<Actor>& asteroids);
	void SetDirection(sf::Vector2f newDir);
	void Draw(sf::RenderWindow& w);

	const sf::Vector2f& GetPos() const;
	float GetRadius() const;

	bool DeepCollision(Line line);
};

