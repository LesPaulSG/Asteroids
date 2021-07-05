#include "Wall.h"

Wall::Wall(sf::Vector2f A, sf::Vector2f B) 
	:	alive(true),
		line(A, B) {
	vector = line.midPoint;
	CalculateRotation();
	body.setPosition(line.pointA);
	body.setSize(sf::Vector2f(1, line.lenght));
	body.setFillColor(sf::Color::White);
	//std::cout << rotation << std::endl;
	//std::cout << "_wC_A_" << line.pointA.x << ' ' << line.pointA.y << std::endl;
	//std::cout << "_wC_B_" << line.pointB.x << ' ' << line.pointB.y << std::endl;
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

void Wall::SetPos(sf::Vector2f pos){
	std::cout << "_w1_A_" << line.pointA.x << ' ' << line.pointA.y << std::endl;
	std::cout << "_w1_B_" << line.pointB.x << ' ' << line.pointB.y << std::endl;
	std::cout << "_pos__" << pos.x << ' ' << pos.y << std::endl;
	line = Line(line.pointA + pos, line.pointB + pos);
	std::cout << "_w2_A_" << line.pointA.x << ' ' << line.pointA.y << std::endl;
	std::cout << "_w2_B_" << line.pointB.x << ' ' << line.pointB.y << std::endl;
	body.setPosition(line.pointA);
}

void Wall::Move(sf::Vector2f dest){
	line.pointA += dest;
	line.pointB += dest;
	body.setPosition(line.pointA);
	//std::cout << body.getPosition().x << ' ' << body.getPosition().y << std::endl;
}

void Wall::RotateAround(const sf::Vector2f& anchor, float angle){
	RotateVectorAround(line.pointA, anchor, angle);
	RotateVectorAround(line.pointB, anchor, angle);
	vector.x = line.pointB.x - line.pointA.x;
	vector.y = line.pointB.y - line.pointA.y;
	CalculateRotation();
	//body.setRotation(rotation * (-180.f / 3.141528f));
	//std::cout << rotation << std::endl;
	sf::Vector2f sdfghEDRH = line.pointA;
	//std::cout << sdfghEDRH.x << ' ' << sdfghEDRH.y << std::endl;
	body.setPosition(sdfghEDRH);

}
