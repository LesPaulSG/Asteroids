#include "Line.h"

Line::Line(sf::Vector2f A, sf::Vector2f B) :
		pointA(A),
		pointB(B)
{
	CaLculateMidPoint();
	CalculateRotation();
	lenght = sqrt(pow(midX, 2) + pow(midY, 2));
}

void Line::CalculateRotation() {
	rotation = acos(midPoint.y / VectorsModule(midPoint));
}

void Line::CaLculateMidPoint(){
	midX = pointB.x - pointA.x;
	midY = pointB.y - pointA.y;
	midPoint.x = midX;
	midPoint.y = midY;
}

bool Line::Intersection(const Line& rhs) const {
	double kA = (midX == 0) ? 0 : midY / midX;
	double kB = (rhs.midX == 0) ? 0 : rhs.midY / rhs.midX;
	if (kA == kB) {
		return false;
	}
	double bA = pointA.y   - kA * pointA.x;
	double bB = rhs.pointA.y - kB * rhs.pointA.x;
	double xInter;
	if (pointB.x == pointA.x) {
		xInter = pointA.x;
	}
	else if (rhs.pointB.x == rhs.pointA.x) {
		xInter = rhs.pointA.x;
	}
	else {
		xInter = (bB - bA) / (kA - kB);
	}
	double yInter = kA * xInter + bA;
	sf::Vector2f result(xInter, yInter);
	if (HasPoint(result) && rhs.HasPoint(result)) {
		return true;
	}
	return false;
}

bool Line::HasPoint(const sf::Vector2f& point) const {
	if (!isPointInRange(point.x, pointA.x, pointB.x)) {
		return false;
	}
	if (!isPointInRange(point.y, pointA.y, pointB.y)) {
		return false;
	}
	return true;
}

float Line::Distance(sf::Vector2f lhs, sf::Vector2f rhs){
	float midX = rhs.x - lhs.x;
	float midY = rhs.y - lhs.y;
	return sqrt(pow(midX, 2) + pow(midY, 2));
}

float Line::Rotation(sf::Vector2f lhs, sf::Vector2f rhs){
	float midX = rhs.x - lhs.x;
	float midY = rhs.y - lhs.y;
	return acos(midY / VectorsModule(sf::Vector2f(midX, midY)));
}