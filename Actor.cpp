#include "Actor.h"

Actor::Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs)
	: pos(pos_),
	dir(dir_),
	speed(50.f),
	alive(true),
	body() {
	if (pairs.size() > 1) {
		body = Polygon(pos, pairs);
		radius = GetRadius();
	}
}

void Actor::Move(float time, std::vector<Actor*>& asteroids) {
	pos += dir * speed * time;
	PassScreenBorder(pos);
	body.Move(pos);
}

bool Actor::Collision(std::vector<Actor*>& asteroids) {
	static float radSum = 0, dist = 0;
	for (auto iter : asteroids) {
		if (iter != this) {
			radSum = radius + iter->GetRadius();
			dist = Line::Distance(pos, iter->GetPos());
			if (radSum >= dist) {
				//std::cout << "collide " << radSum << ' ' << dist << std::endl;
				return true;
			}
		}
	}
	return false;
}

void Actor::SetDirection(sf::Vector2f newDir){
	dir = newDir;
}

void Actor::Draw(sf::RenderWindow& w) {
	body.Draw(w);
}

void Actor::Destroy(){
	alive = false;
}

const sf::Vector2f& Actor::GetPos() const {
	return pos;
}

float Actor::GetRadius() const {
	return radius;
}

bool Actor::isAlive(){
	return alive;
}

bool Actor::DeepCollision(Line line) {
	return body.isCollision(line);
}