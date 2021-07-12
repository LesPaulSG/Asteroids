#include "Actor.h"

Actor::Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs) :
		body(pos, pairs),
		pos(pos_),
		dir(dir_),
		speed(900.f),
		alive(true),
		playerKiiled(false)	
{

}

void Actor::Update(float time, std::vector<Actor*>& actors) {
	pos += dir * speed * time;
	PassScreenBorder(pos);
	body.Move(pos);
}

bool Actor::Collision(const std::vector<Actor*>& actors) {
	static float radSum = 0, dist = 0;
	for (auto iter : actors) {
		if (iter != this) {			//check for dont collide with self
			radSum = GetBodyRadius() + iter->GetBodyRadius();	//sum of radius
			dist   = Line::Distance(pos, iter->GetPos());		//distance between actors
			if (radSum >= dist) {								//kind of "bounding sphere" collision detection
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

bool Actor::DeepCollision(const Line& line) const {
	return body.isCollision(line);
}