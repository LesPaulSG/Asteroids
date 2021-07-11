#include "Line.h"

Line::Line(sf::Vector2f A, sf::Vector2f B) :
		pointA(A),
		pointB(B)
{
	CalculateRotation();
	lenght = sqrt(pow(midX, 2) + pow(midY, 2));
}

Line::~Line() {}

//rework
void Line::CalculateRotation() {
	midX = pointB.x - pointA.x;
	midY = pointB.y - pointA.y;
	midPoint.x = midX;
	midPoint.y = midY;
	rotation = acos(midPoint.y / VectorsModule(midPoint));
}

bool Line::Intersection(const Line& B) const {
	double kA = (midX == 0 || midY == 0) ? 0 :midY / midX;
	double kB = (B.midX == 0 || B.midY == 0) ? 0 :B.midY / B.midX;
	if (kA == kB) {
		return false;
	}
	double bA = pointA.y   - kA * pointA.x;
	double bB = B.pointA.y - kB * B.pointA.x;
	double xInter;
	if (pointB.x == pointA.x) {
		xInter = pointA.x;
	}
	else if (B.pointB.x == B.pointA.x) {
		xInter = B.pointA.x;
	}
	else {
		xInter = (bB - bA) / (kA - kB);
	}
	double yInter = kA * xInter + bA;
	sf::Vector2f result(xInter, yInter);
	if (HasPoint(result) && B.HasPoint(result)) {
		return true;
	}
	return false;
}

float Line::AngleOfIntersec(const Line& B) const {
	sf::Vector2f first(midPoint), second(B.midPoint);

	float fMod = VectorsModule(first);
	float sMod = VectorsModule(second);

	return acos((first.x * second.x + first.y * second.y) / (fMod * sMod));
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

float Line::Distance(sf::Vector2f from, sf::Vector2f to){
	float midX = to.x - from.x;
	float midY = to.y - from.y;
	return sqrt(pow(midX, 2) + pow(midY, 2));
}

float Line::Rotation(sf::Vector2f from, sf::Vector2f to){
	float midX = to.x - from.x;
	float midY = to.y - from.y;
	return acos(midY / VectorsModule(sf::Vector2f(midX, midY)));
}

float Line::CalculateRotation(sf::Vector2f A, sf::Vector2f B){
	sf::Vector2f mid;
	mid.x = B.x - A.x;
	mid.y = B.y - A.y;
	double angle = acos(mid.y / VectorsModule(mid));
	return angle;
}

//function checks point position relative to line (left or right)
bool Line::isPointRight(const sf::Vector2f& point) const {
	float D = (point.x - pointA.x) * midY - (point.y - pointA.y) * midX;
	if (D > 0) {
		return false;
	}
	return true;
}