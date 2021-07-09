#pragma once
#include "Wall.h"

class Polygon{
private:
	std::vector<Wall> edges;
	sf::Vector2f pos;
	float radius = 0;
	float rotation;
public:
	Polygon();
	Polygon(sf::Vector2f pos_, const std::vector<VectorPair>& pairs);
	void Draw(sf::RenderWindow& w) const;
	void Move(sf::Vector2f dest);
	void Rotate(float angle);
	bool isCollision(const Line& line) const;
	void Explode();

	float GetRot() const;
	float GetRadius() const;
};

