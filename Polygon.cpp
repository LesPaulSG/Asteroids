#include "Polygon.h"

Polygon::Polygon() 
	: pos(sf::Vector2f(0, 0)) {

}

Polygon::Polygon(sf::Vector2f pos_, std::vector<sf::Vector2f> points)
	: pos(pos_) {
	int pointsNum = points.size();
	edges.reserve(pointsNum);
	offsets.reserve(pointsNum);

	for (auto point : points) {
		radius = (radius < point.x) ? point.x : radius;
		radius = (radius < point.y) ? point.y : radius;
	}
	std::cout << "------------------------------------------------------------\n";
	for (int i = 0; i < pointsNum - 1; ++i) {
		std::cout << points.at(i).x << ' ' << points.at(i).y << std::endl;
		std::cout << points.at(i + 1).x << ' ' << points.at(i + 1).y << std::endl;
		edges.push_back(Wall(points.at(i), points.at(i + 1)));
	}
	std::cout << points.at(0).x << ' ' << points.at(0).y << std::endl;
	std::cout << points.at(points.size() - 1).x << ' ' << points.at(points.size() - 1).y << std::endl;
	std::cout << "------------------------------------------------------------\n";

	edges.push_back(Wall(points.at(0), points.at(pointsNum - 1)));

	/*std::cout << "LINE 1 POINT A " << edges[0].GetLine().pointA.x << "\t" << edges[0].GetLine().pointA.y << std::endl;
	std::cout << "LINE 1 POINT B " << edges[0].GetLine().pointB.x << "\t" << edges[0].GetLine().pointB.y << std::endl;
	std::cout << "LINE 2 POINT A " << edges[1].GetLine().pointA.x << "\t" << edges[1].GetLine().pointA.y << std::endl;
	std::cout << "LINE 2 POINT B " << edges[1].GetLine().pointB.x << "\t" << edges[1].GetLine().pointB.y << std::endl;
	std::cout << "LINE 3 POINT A " << edges[2].GetLine().pointA.x << "\t" << edges[2].GetLine().pointA.y << std::endl;
	std::cout << "LINE 3 POINT B " << edges[2].GetLine().pointB.x << "\t" << edges[2].GetLine().pointB.y << std::endl;*/

	for (auto& edge : edges) {
		//std::cout << edge.GetBody().getPosition().x << ' ' << edge.GetBody().getPosition().y << std::endl;
		//edge.SetCustomOrigin(edge.GetBody().getPosition()*2.f);
		//std::cout << edge.GetBody().getOrigin().x << ' ' << edge.GetBody().getOrigin().y << std::endl;
		offsets.push_back(edge.GetLine().pointB);
	}
	//Move(pos);
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
		edge.Move(pos - edge.GetLine().pointB);
	}
	/*edges[0].Move(pos + sf::Vector2f(-10, 10));
	edges[1].Move(pos + sf::Vector2f(10, 10));
	edges[2].Move(pos + sf::Vector2f(10, 10));*/
}

void Polygon::Rotate(float angle){
	for (auto& edge : edges) {
		edge.RotateAround(pos, 0.000001f);
	}
}
