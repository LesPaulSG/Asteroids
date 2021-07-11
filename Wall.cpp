#include "Wall.h"

Wall::Wall(sf::Vector2f A, sf::Vector2f B) :
		offsetA(A),
		offsetB(B),
		line(A, B),
		body(sf::Vector2f(1.5f, line.lenght))
{
	vector = line.midPoint;
	CalculateRotation();
	body.setPosition(line.pointA);
	body.setOrigin(0.75f, 0);
	body.setFillColor(sf::Color::White);
}

const sf::RectangleShape& Wall::GetBody() const {return body;}

const Line& Wall::GetLine() const {return line;}

void Wall::AddOffset(float deltaTime){
	offsetA *= 1.000023f;
	offsetB *= 1.000023f;
}

//TODO use lines rotation
void Wall::CalculateRotation() {
	double angle = line.rotation;
	angle *= -(180.f / 3.141528f);
	if (line.pointA.x > line.pointB.x) {
		angle *= -1.f;
	}
	body.setRotation(angle);
}

void Wall::Move(sf::Vector2f dest){
	line.pointA = dest + offsetA;
	line.pointB = dest + offsetB;
	CalculateRotation();
	body.setPosition(line.pointA);
}

void Wall::RotateAround(const sf::Vector2f& anchor, float angle){
	RotateVector(line.pointA, angle, anchor);
	RotateVector(line.pointB, angle, anchor);
	RotateVector(offsetA, angle);
	RotateVector(offsetB, angle);
	vector.x = line.pointB.x - line.pointA.x;
	vector.y = line.pointB.y - line.pointA.y;
	line.CalculateRotation();
	CalculateRotation();
	body.setPosition(line.pointA);
}
