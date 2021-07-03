#include "Polygon.h"

Polygon::Polygon(sf::Vector2f pos_, std::vector<sf::Vector2f> points)
	: pos(pos_) {
	edges.reserve(POLYGON_MAX_SIZE);
	for (auto point : points) {
		radius = (radius < point.x) ? point.x : radius;
		radius = (radius < point.y) ? point.y : radius;
	}
	for (int i = 0; i < points.size() - 1; ++i) {
		std::cout << points.at(i).x << ' ' << points.at(i).y << std::endl;
		std::cout << points.at(i + 1).x << ' ' << points.at(i + 1).y << std::endl;
		edges.push_back(Wall(points.at(i), points.at(i + 1)));
	}
	std::cout << points.at(0).x << ' ' << points.at(0).y << std::endl;
	std::cout << points.at(points.size() - 1).x << ' ' << points.at(points.size() - 1).y << std::endl;
	edges.push_back(Wall(points.at(0), points.at(points.size() - 1)));
	for (auto& edge : edges) {
		//std::cout << edge.GetBody().getPosition().x << ' ' << edge.GetBody().getPosition().y << std::endl;
		//edge.SetCustomOrigin(edge.GetBody().getPosition()*2.f);
		//std::cout << edge.GetBody().getOrigin().x << ' ' << edge.GetBody().getOrigin().y << std::endl;
	}
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
	for (auto& edge : edges) {
		//std::cout << edge.GetBody().getPosition().x << ' ' << edge.GetBody().getPosition().y << std::endl;
		//edge.Move(pos - edge.GetBody().getOrigin());
	}
	edges[0].Move(pos - sf::Vector2f(10, -10));
	edges[1].Move(pos - sf::Vector2f(-10, -10));
	//edges[2].Move(pos - sf::Vector2f(-10, -10));
}
