#pragma once
#include "Polygon.h"

class Actor {
protected:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	Polygon body;
	bool alive;
	
public:
	Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs);
	virtual ~Actor() = default;

	virtual void Move(float time, std::vector<Actor*>& asteroids);
	virtual bool Collision(const std::vector<Actor*>& asteroids);
	virtual void Draw(sf::RenderWindow& w) const;
	virtual void Destroy();

	const sf::Vector2f& GetPos() const;
	float GetBodyRadius() const;
	virtual bool isAlive() const;

	bool DeepCollision(Line line) const;
};

