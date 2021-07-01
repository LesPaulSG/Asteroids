#include "Wall.h"

Wall::Wall(sf::Vector2f A, sf::Vector2f B) 
	:	alive(true),
		line(A, B) {
	vector = line.midPoint;
	CalculateRotation();
	body.setPosition(line.pointA);
	body.setSize(sf::Vector2f(5, line.lenght));
	body.setFillColor(sf::Color::White);
}

bool Wall::GetAlive() const {return alive;}

const sf::RectangleShape& Wall::GetBody() const {return body;}

const Line& Wall::GetLine() const {return line;}

double Wall::GetRotation() const {return rotation;}

void Wall::Destroy() { alive = false; }

void Wall::CalculateRotation() {
	double angle = acos(vector.y / VectorsModule(vector));
	rotation = angle;
	angle *= -(180.f / 3.141528f);
	if (line.pointA.x > line.pointB.x) {
		angle *= -1.f;
	}
	body.setRotation(angle);
}

void Wall::SetCustomOrigin(sf::Vector2f newOrig){
	body.setOrigin(newOrig.x, newOrig.y);
}

void Wall::Move(sf::Vector2f dest){
	body.setPosition(dest);
	//std::cout << body.getPosition().x << ' ' << body.getPosition().y << std::endl;
}
