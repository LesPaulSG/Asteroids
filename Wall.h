#pragma once
#include "Line.h"

class Wall {
private:
	bool alive;
	sf::Vector2f vector;
	sf::Vector2f offsetA;
	sf::Vector2f offsetB;
	sf::RectangleShape body;
	Line line;
	double rotation;

public:
	Wall(sf::Vector2f A, sf::Vector2f B);
	~Wall() = default;

	bool GetAlive() const;
	const sf::RectangleShape& GetBody() const;
	const Line& GetLine() const;
	double GetRotation() const;

	void Destroy();
	void CalculateRotation();
	void SetCustomOrigin(sf::Vector2f newOrig);
	void SetPos(sf::Vector2f pos);
	void Move(sf::Vector2f dest);
	void RotateAround(const sf::Vector2f& anchor, float angle);
};