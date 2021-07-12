#pragma once
#include "Line.h"

class Wall {
private:
	sf::RectangleShape body;
	Line line;
	sf::Vector2f vector;
	sf::Vector2f offsetA;
	sf::Vector2f offsetB;
	double rotation;

public:
	Wall(sf::Vector2f A, sf::Vector2f B);
	~Wall() = default;

	const sf::RectangleShape& GetBody() const;
	const Line& GetLine() const;
	void AddOffset(float deltaTime);

	void RotateBody();
	void Move(sf::Vector2f dest);
	void RotateAround(const sf::Vector2f& anchor, float angle);
};