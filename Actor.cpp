#include "Actor.h"

Actor::Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<sf::Vector2f>& points)
	: pos(pos_),
	dir(dir_),
	speed(50.f),
	body(pos, points) {
	radius = 0; //add get radius in polygon

}

void Actor::Move(float time, std::vector<Actor>& asteroids) {
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir * speed * time;			//new position
	//Collision(asteroids);
	PassScreenBorder(pos);
	body.Move(pos);
}

bool Actor::Collision(std::vector<Actor>& asteroids) {
	static float radSum = 0, dist = 0;
	for (auto& iter : asteroids) {
		if (&iter != this) {
			radSum = radius + iter.GetRadius();
			dist = Line(pos, iter.GetPos()).lenght;
			if (radSum >= dist) {
				//std::cout << "collide " << radSum << ' ' << dist << std::endl;
				return true;
			}
		}
	}
	return false;
}

void Actor::Draw(sf::RenderWindow& w) {
	body.Draw(w);
}

const sf::Vector2f& Actor::GetPos() const {
	return pos;
}

float Actor::GetRadius() const {
	return radius;
}

bool Actor::DeepCollision(Line line) {
	return body.isCollision(line);
}
