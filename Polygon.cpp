#include "Polygon.h"

std::vector<float> r;

Polygon::Polygon() 
	: pos(sf::Vector2f(0, 0)) {
}

Polygon::Polygon(sf::Vector2f pos_, std::vector<sf::Vector2f> points)
	: pos(pos_),
	rotation(0) {
	int pointsNum = points.size();
	edges.reserve(pointsNum);
	offsets.reserve(pointsNum);

	for (auto point : points) {
		radius = (radius < point.x) ? point.x : radius;
		radius = (radius < point.y) ? point.y : radius;
	}
	for (int i = 0; i < pointsNum - 1; ++i) {
		edges.push_back(Wall(points.at(i), points.at(i + 1)));
	}
	edges.push_back(Wall(points.at(pointsNum - 1), points.at(0)));

	for (auto& edge : edges) {
		offsets.push_back(edge.GetLine().pointB);
		r.push_back(edge.GetRotation());
		edge.SetPos(pos);
	}
}

void Polygon::Draw(sf::RenderWindow& w) const{
	for (auto edge : edges) {
		w.draw(edge.GetBody());
	}
}

void Polygon::Move(sf::Vector2f dest){
	sf::Vector2f diff = dest - pos;
	pos = dest;
	for (auto& edge : edges) {
		edge.Move(diff);
	}
}

void Polygon::Rotate(float angle){
	for (auto& edge : edges) {
		edge.RotateAround(pos, angle);
	}
}

std::vector<Wall>& Polygon::getEdges() {
	return edges;
}

float Polygon::GetRot(){
	return rotation;
}
