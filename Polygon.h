#pragma once
#include "Wall.h"

class Polygon{
private:
	std::vector<Wall> edges;
	sf::Vector2f pos;
	float radius;
	float rotation;
public:
	Polygon();
	Polygon(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs);
	void Draw(sf::RenderWindow& w) const;
	void Move(sf::Vector2f dest);
	void Rotate(float angle);
	void Explode(float deltaTime);
	void Reset(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs);

	float GetRot() const;
	float GetRadius() const;
	const std::vector<Wall>& GetEdges() const;
	bool isCollision(const Line& line) const;
};