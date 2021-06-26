#pragma once
#include "Functions.h"

class Line {
public:
	sf::Vector2f pointA;
	sf::Vector2f pointB;
	float midX;
	float midY;
	sf::Vector2f midPoint;
	float lenght;				//check grammar
	double rotation;

	Line(sf::Vector2f A, sf::Vector2f B);
	~Line();

	void CalculateRotation();

	bool isPointRight(const sf::Vector2f& point) const;
	bool Intersection(const Line& B, sf::Vector2f& iPoint) const;
	bool CircleIntersection(const sf::Vector2f& circle, float radius, sf::Vector2f& iPoint) const;
	float AngleOfIntersec(const Line& B) const;
	bool HasPoint(const sf::Vector2f& point) const;

	static float CalculateRotation(sf::Vector2f A, sf::Vector2f B);
};