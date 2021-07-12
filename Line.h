#pragma once
#include "Functions.h"

struct Line {
	sf::Vector2f pointA;
	sf::Vector2f pointB;
	sf::Vector2f midPoint;
	double rotation;
	float midX;
	float midY;
	float lenght;

	Line(sf::Vector2f A, sf::Vector2f B);

	void CalculateRotation();
	void CaLculateMidPoint();

	bool Intersection(const Line& rhs) const;
	bool HasPoint(const sf::Vector2f& point) const;

	static float Distance(sf::Vector2f lhs, sf::Vector2f rhs);
	static float Rotation(sf::Vector2f lhs, sf::Vector2f rhs);
};