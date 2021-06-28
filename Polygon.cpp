#include "Polygon.h"

Polygon::Polygon(sf::Vector2f pos_, std::vector<sf::Vector2f> points)
	: pos(pos_) {
	edges.reserve(POLYGON_MAX_SIZE);
	for (auto point : points) {
		radius = (radius < point.x) ? point.x : radius;
		radius = (radius < point.y) ? point.y : radius;
	}
	for (int i = 0; i < points.size() - 1; ++i) {
		edges.push_back(Wall(points.at(i), points.at(i + 1)));
	}
	edges.push_back(Wall(points.at(0), points.at(points.size() - 1)));
	for (auto edge : edges) {
		//edge.SetCustomOrigin(pos + edge.GetBody().getPosition());
	}
	edges[0].SetCustomOrigin(pos / 2.f);
	edges[1].SetCustomOrigin(pos / 2.f);
	edges[2].SetCustomOrigin(pos / 2.f);
	Move(pos);
}

void Polygon::Draw(sf::RenderWindow& w) const{
	for (auto edge : edges) {
		w.draw(edge.GetBody());
	}
}

void Polygon::Move(sf::Vector2f dest){
	sf::Vector2f diff = dest - pos;
	pos = dest;
	//std::cout << pos.x << ' ' << pos.y << std::endl;
	for (auto edge : edges) {
		edge.Move(pos);
	}
	edges[0].Move(edges[0].GetBody().getPosition() + diff);
	edges[1].Move(edges[1].GetBody().getPosition() + diff);
	edges[2].Move(edges[2].GetBody().getPosition() + diff);
}
