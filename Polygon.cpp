#include "Polygon.h"

Polygon::Polygon() : pos(sf::Vector2f(0.f, 0.f)) {}

Polygon::Polygon(sf::Vector2f pos_, const std::vector<VectorPair>& pairs) :
		pos(pos_),
		rotation(0)
{
	edges.reserve(pairs.size());

	for (auto pair : pairs) {
		radius = std::max(radius, std::abs(pair.f.x));
		radius = std::max(radius, std::abs(pair.f.y));
		radius = std::max(radius, std::abs(pair.s.x));
		radius = std::max(radius, std::abs(pair.s.y));
		edges.push_back(Wall(pair.f, pair.s));
	}

	for (auto& edge : edges) {
		edge.Move(pos);
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
	if (angle != rotation) {
		for (auto& edge : edges) {
			edge.RotateAround(pos, rotation - angle);
		}
		rotation = angle;
	}
}

bool Polygon::isCollision(const Line& line) const{
	for (auto& iter : edges) {
		if (line.Intersection(iter.GetLine()))
			return true;
	}
	return false;
}

void Polygon::Explode(float deltaTime){
	for (auto& edge : edges) {
		edge.AddOffset(deltaTime);
	}
}

float Polygon::GetRot() const {
	return rotation;
}

float Polygon::GetRadius() const {
	return radius;
}
