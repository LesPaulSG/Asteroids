#include "Asteroid.h"

std::uniform_int_distribution<int> dist(5, 15);
std::uniform_real_distribution<float> rot(0.5, 1.0);

Asteroid::Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_)
	: pos(pos_),
	dir(dir_),
	speed(50.f),
	stage(stage_),
	alive(true) {
	radius = stage * 15;

	std::vector<sf::Vector2f> points;
	points.reserve(3 * stage);
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
	body = Polygon(pos, points);
}

void Asteroid::Move(float time, std::vector<Asteroid>& asteroids){
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir * speed * time;			//new position
	CheckCollision(asteroids);
	PassScreenBorder(pos);
	body.Move(pos);
}

void Asteroid::CheckCollision(std::vector<Asteroid>& asteroids){
	static float radSum = 0, dist = 0;
	for (auto& iter : asteroids) {
		if (&iter != this) {
			radSum = radius + iter.GetRadius();
			dist = Line(pos, iter.GetPos()).lenght;
			if (radSum >= dist) {
				//std::cout << "collide " << radSum << ' ' << dist << std::endl;
			}
		}
	}
}

void Asteroid::UpdateDirection(sf::Vector2f newDir){
	dir = newDir;
}

void Asteroid::Draw(sf::RenderWindow& w){
	body.Draw(w);
}

void Asteroid::Destroy(){
	alive = false;
}

const sf::Vector2f& Asteroid::GetPos() const{
	return pos;
}

float Asteroid::GetRadius() const{
	return radius;
}

bool Asteroid::GetAlive() const {
	return alive;
}

int Asteroid::GetStage() const {
	return stage;
}
