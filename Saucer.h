#pragma once
#include "Polygon.h"

class Saucer {
private:
	sf::Vector2f pos;
	sf::Vector2f dir;
	float speed;
	Polygon body;
	bool big;
public:
	Saucer(sf::Vector2f p, bool b);
	void Update(float deltaTime);
	void Draw(sf::RenderWindow& w);

	bool CanShoot(float deltaTime);
};