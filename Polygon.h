#pragma once
#include "Wall.h"

class Polygon{
private:
	std::vector<Wall> edges;
	sf::Vector2f pos;
	float radius = 0;
public:
	Polygon(sf::Vector2f pos_, std::vector<sf::Vector2f> points);
	void Draw(sf::RenderWindow& w) const;
	void Move(sf::Vector2f dest);
};

