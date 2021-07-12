#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_) : 
		Actor(pos_, dir_, {}),
		stage(stage_),
		alive(true) 
{
	speed *= 3.f / stage;
	std::vector<sf::Vector2f> points;	//random ganarated points
	points.reserve(ASTEROID_MAX_SIZE);

	//generating points with random x offset, y == 0
	for (int i = 0; i < ASTEROID_MAX_SIZE; ++i) {
		sf::Vector2f p(RAND_ASTEROID_RADIUS(gen) * stage, 0);
		points.push_back(p);
	}

	//rotating this points around pos
	float prevAngle = 0;
	for (int i = 1; i < ASTEROID_MAX_SIZE; ++i) {
		float angle = RAND_ANGLE_HALF_RAD(gen);
		RotateVector(points[i], prevAngle + angle);
		prevAngle += angle;
	}

	std::vector<VectorPair> pairs;
	pairs.reserve(ASTEROID_MAX_SIZE);

	//creating random polygon pattern from point gained upper
	for (int i = 1; i < ASTEROID_MAX_SIZE; ++i) {
		pairs.push_back(VectorPair(points[i-1], points[i]));
	}
	//last pair connecting first and last point, so polygon always will be closed
	pairs.push_back(VectorPair(points.back(), points.front()));

	body = Polygon(pos, pairs);
}

int Asteroid::GetStage() const {return stage;}

Values Asteroid::GetCost() const {
	switch (stage) {
	case 1: return Values::ASTR_S;
	case 2: return Values::ASTR_M;
	case 3: return Values::ASTR_B;
	}
}
