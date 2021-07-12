#include "Polygon.h"

Polygon::Polygon() : pos(sf::Vector2f(0.f, 0.f)) {}

Polygon::Polygon(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs) :
		pos(pos_),
		radius(0.f),
		rotation(0.f) 
{
	edges.reserve(pairs.size());

	for (auto pair : pairs) {
		radius = std::max(radius, std::abs(pair.first.x));
		radius = std::max(radius, std::abs(pair.first.y));
		radius = std::max(radius, std::abs(pair.second.x));
		radius = std::max(radius, std::abs(pair.second.y));
		edges.push_back(Wall(pair.first, pair.second));
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
			edge.RotateAround(pos, angle - rotation);
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

void Polygon::Reset(const sf::Vector2f& pos_, const std::vector<VectorPair>& pairs){
	pos = pos_;
	rotation = 0.f;
	if (pairs.size() != edges.size()) return;
	for (int i = 0; i < pairs.size(); ++i) {
		edges.at(i).ResetOffset(pairs.at(i));
		edges.at(i).Move(pos);
		edges.at(i).RotateAround(pos, rotation);
	}
}

float Polygon::GetRot() const {return rotation;}

float Polygon::GetRadius() const {return radius;}

const std::vector<Wall>& Polygon::GetEdges() const {return edges;}
