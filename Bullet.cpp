#include "Bullet.h"

#include <iostream>

Bullet::Bullet(Shot sho)
	: pos(sho.from),
	speed(600.f),
	time(0.f),
	lifeTime(1.f),
	alive(true) {
	RotateUnitVector(this->dir, sho.angle);
	body.setPosition(pos);
	body.setRadius(1);
	body.setFillColor(sf::Color::White);
}

const sf::CircleShape& Bullet::GetBody() const {return body;}

bool Bullet::isAlive() const {return alive;}

void Bullet::CheckCollision(std::vector<Actor*>& actors, const sf::Vector2f& oldPos) {
	static float dist = 0;
	sf::Vector2f iPoint(0.f, 0.f);
	Line offset(oldPos, pos);
	for (auto& iter : actors){
		dist = Line::Distance(pos, iter->GetPos());
		if (dist <= iter->GetRadius()) {
			if (iter->DeepCollision(offset)) {
				iter->Destroy();
				alive = false;
				return;
			}
		}
	}
}

void Bullet::Update(float t, std::vector<Actor*>& actors) {
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir *speed* t;				//new position
	if (PassScreenBorder(pos)) {
		alive = false;
		return;
	}
	body.setPosition(pos);
	CheckCollision(actors, oldPos);

	speed -= t/30.f;					//braking over time
	time  += t;
		
	LifeCheck();
}

void Bullet::PushForward(float distance){
	pos += dir * distance;
}

void Bullet::LifeCheck(){
	if (time >= lifeTime) {
		alive = false;
	} else if (speed <= 0.f) {
		alive = false;
	}
}