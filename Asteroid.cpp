#include "Asteroid.h"

std::uniform_int_distribution<int> dist(10, 20);
std::uniform_real_distribution<float> rot(0.5, 1.0);

Asteroid::Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_): 
	Actor(pos_, dir_, {}),
	stage(stage_),
	alive(true) {

	std::vector<sf::Vector2f> points;
	points.reserve(8);
	float prevAngle = 0;
	for (int i = 0; i < 8; ++i) {
		sf::Vector2f p(dist(gen) * stage, 0);
		points.push_back(p);
	}
	for (int i = 1; i < 8; ++i) {
		float angle = rot(gen);
		RotateVector(points[i], prevAngle + angle);
		prevAngle += angle;
	}
	std::vector<VectorPair> pairs;
	pairs.reserve(8);
	for (int i = 1; i < 8; ++i) {
		pairs.push_back(VectorPair(points[i-1], points[i]));
	}
	pairs.push_back(VectorPair(points[7], points[0]));
	body = Polygon(pos, pairs);
	radius = body.GetRadius();
}

int Asteroid::GetStage() const {
	return stage;
}