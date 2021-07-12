#include "Actor.h"

Actor::Actor(sf::Vector2f pos_, sf::Vector2f dir_, const std::vector<VectorPair>& pairs) :
		body(pos, pairs),
		pos(std::move(pos_)),
		dir(std::move(dir_)),
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

bool Actor::Collision(const std::vector<Actor*>& actors) const {
	static float radSum = 0, dist = 0;
	for (auto iter : actors) {
		if (iter != this) {			//check for dont collide with self
			radSum = GetRadius() + iter->GetRadius();	//sum of radius
			dist   = Line::Distance(pos, iter->GetPos());		//distance between actors
			if (radSum >= dist) {								//kind of "bounding sphere" collision detection
				for (auto& edge : body.GetEdges()) {
					if (iter->DeepCollision(edge.GetLine())) {
						iter->Destroy();
						return true;
					}
				}
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

float Actor::GetRadius() const {return body.Polygon::GetRadius();}

bool Actor::isAlive() const {return alive;}

bool Actor::isPlayerDestoyed() const {return playerKiiled;}

bool Actor::DeepCollision(const Line& line) const {
	return body.isCollision(line);
}