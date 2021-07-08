#include "Polygon.h"

std::vector<float> r;

Polygon::Polygon() 
	: pos(sf::Vector2f(0, 0)) {
}

Polygon::Polygon(sf::Vector2f pos_, const std::vector<VectorPair>& pairs)
	: pos(pos_),
	rotation(0) {
	int pointsNum = pairs.size();

	edges.reserve(pointsNum);
	offsets.reserve(pointsNum);

	for (auto pair : pairs) {
		radius = std::max(radius, pair.f.x);
		radius = std::max(radius, pair.f.y);
		radius = std::max(radius, pair.s.x);
		radius = std::max(radius, pair.s.y);
		edges.push_back(Wall(pair.f, pair.s));
	}

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

bool Polygon::isCollision(Line line){
	sf::Vector2f iPoint(0, 0);
	for (auto& iter : edges) {
		if (line.Intersection(iter.GetLine(), iPoint))
			return true;
	}
	return false;
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
