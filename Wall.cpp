#include "Wall.h"

Wall::Wall(sf::Vector2f A, sf::Vector2f B) :
		offsetA(A),
		offsetB(B),
		line(A, B),
		body(sf::Vector2f(1.f, line.lenght))
{
	vector = line.midPoint;
	CalculateRotation();
	body.setPosition(line.pointA);
	body.setFillColor(sf::Color::White);
}

const sf::RectangleShape& Wall::GetBody() const {return body;}

const Line& Wall::GetLine() const {return line;}

void Wall::AddOffset(){
	offsetA *= 1.000023f;
	offsetB *= 1.000023f;
}

void Wall::CalculateRotation() {
	double angle = acos(vector.y / VectorsModule(vector));
	rotation = angle;
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
	RotateVectorAround(line.pointA, anchor, angle);
	RotateVectorAround(line.pointB, anchor, angle);
	RotateVectorAround(offsetA, sf::Vector2f(0.f, 0.f), angle);
	RotateVectorAround(offsetB, sf::Vector2f(0.f, 0.f), angle);
	vector.x = line.pointB.x - line.pointA.x;
	vector.y = line.pointB.y - line.pointA.y;
	CalculateRotation();
	body.setPosition(line.pointA);
}
