#include "Bullet.h"

#include <iostream>

Bullet::Bullet(sf::Vector2f pos, float dir, float speed, float lifeTime)
	: pos(std::move(pos)),
	speed(600.f),
	time(0.f),
	lifeTime(lifeTime),
	alive(true) {
	RotateUnitVector(this->dir, dir);
	body.setPosition(pos);
	body.setRadius(1);
	body.setFillColor(sf::Color::White);
}

const sf::CircleShape& Bullet::GetBody() const {return body;}

bool Bullet::GetAlive() const {return alive;}

void Bullet::CheckCollision(std::vector<Asteroid>& asteroids, const sf::Vector2f& oldPos) {
	sf::Vector2f iPoint(0.f, 0.f);
	Line offset(oldPos, pos);
	for (auto& iter : asteroids){
		if (Line(pos, iter.GetPos()).lenght <= iter.GetRadius()) {
			if (iter.isCollision(offset)) {
				iter.Destroy();
				alive = false;
				return;
			}
		}
		//old collision detectcion
		/*if (iter.GetAlive()) {
			if (offset.Intersection(iter.GetLine(), iPoint)) {
				alive = false;
				iter.Destroy();
				return;
			}
		}*/
	}
}

void Bullet::Update(float t, std::vector<Asteroid>& asteroids) {
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir *speed* t;				//new position
	if (PassScreenBorder(pos)) {
		alive = false;
		return;
	}
	body.setPosition(pos);
	CheckCollision(asteroids, oldPos);

	speed -= t/30.f;					//braking over time
	time  += t;
		
	LifeCheck();
}

void Bullet::LifeCheck(){
	if (time >= lifeTime) {
		alive = false;
	} else if (speed <= 0.f) {
		alive = false;
	}
}