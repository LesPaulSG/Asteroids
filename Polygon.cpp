#include "Polygon.h"

Polygon::Polygon() 
	: pos(sf::Vector2f(0, 0)) {
}

Polygon::Polygon(sf::Vector2f pos_, const std::vector<VectorPair>& pairs)
	: pos(pos_),
	rotation(0) {
	int pointsNum = pairs.size();
	edges.reserve(pointsNum);

	for (auto pair : pairs) {
		radius = std::max(radius, std::abs(pair.f.x));
		radius = std::max(radius, std::abs(pair.f.y));
		radius = std::max(radius, std::abs(pair.s.x));
		radius = std::max(radius, std::abs(pair.s.y));
		edges.push_back(Wall(pair.f, pair.s));
	}

	for (auto& edge : edges) {
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
		edge.Move(pos);
	}
}

void Polygon::Rotate(float angle){
	for (auto& edge : edges) {
		edge.RotateAround(pos, angle);
	}
}

bool Polygon::isCollision(Line line){
	sf::Vector2f iPoint(0, 0);
	for (auto& iter : edges) {
		if (line.Intersection(iter.GetLine(), iPoint))
			return true;
	}
	return false;
}

void Polygon::Explode(){
	for (auto& edge : edges) {
		edge.AddOffset();
	}
}

std::vector<Wall>& Polygon::getEdges() {
	return edges;
}

float Polygon::GetRot(){
	return rotation;
}

float Polygon::GetRadius(){
	return radius;
}
