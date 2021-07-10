#include "Actor.h"

Actor::Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs) :
		pos(pos_),
		dir(dir_),
		speed(50.f),
		alive(true),
		playerKiiled(false),
		body(pos, pairs)
{

}

void Actor::Move(float time, std::vector<Actor*>& asteroids) {
	pos += dir * speed * time;
	PassScreenBorder(pos);
	body.Move(pos);
}

bool Actor::Collision(const std::vector<Actor*>& asteroids) {
	static float radSum = 0, dist = 0;
	for (auto iter : asteroids) {
		if (iter != this) {			//check for dont collide with seld
			radSum = GetBodyRadius() + iter->GetBodyRadius();			//sum of radius
			dist   = Line::Distance(pos, iter->GetPos());	//distance between actors
			if (radSum >= dist) {							//kind of "bounding sphere" collision detection
				return true;
			}
		}
	}
	return false;
}

void Actor::Draw(sf::RenderWindow& w) const {
	body.Draw(w);
}

void Actor::Destroy(bool playerDestroy){
	alive = false;
	playerKiiled = playerDestroy;
}

const sf::Vector2f& Actor::GetPos() const {return pos;}

float Actor::GetBodyRadius() const {return body.GetRadius();}

bool Actor::isAlive() const {return alive;}

bool Actor::isPlayerDestoyed() const {return playerKiiled;}

bool Actor::DeepCollision(Line line) const {
	return body.isCollision(line);
}