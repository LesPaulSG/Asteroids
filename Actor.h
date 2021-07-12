#pragma once
#include "Polygon.h"

class Actor {
protected:
	Polygon body;
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	bool alive;
	bool playerKiiled;
public:
	Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs);

	virtual void Update(float time, std::vector<Actor*>& actors);
	virtual bool Collision(const std::vector<Actor*>& actors);
	virtual void Draw(sf::RenderWindow& w) const;
	virtual void Destroy(bool playerDestroy = false);
	
	const sf::Vector2f& GetPos() const;
	float GetBodyRadius() const;
	virtual bool isAlive() const;
	bool isPlayerDestoyed() const;

	bool DeepCollision(const Line& line) const;
};

