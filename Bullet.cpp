#include "Bullet.h"

#include <iostream>

Bullet::Bullet(sf::Vector2f pos, float dir, float speed, float lifeTime)
	: pos(std::move(pos)),
	speed(50),
	time(0.f),
	lifeTime(lifeTime),
	alive(true) {
	RotateUnitVector(this->dir, dir);
	body.setPosition(pos);
	body.setSize(sf::Vector2f(1, 10));
	double angle = acos(this->dir.y / VectorsModule(this->dir));
	dir *= -(180.f / 3.141528f);
	body.setRotation(dir);
	body.setFillColor(sf::Color::Red);
}

const sf::RectangleShape& Bullet::GetBody() const {return body;}

bool Bullet::GetAlive() const {return alive;}

void Bullet::CheckCollision(std::vector<Wall>& walls, const sf::Vector2f& oldPos) {
	sf::Vector2f iPoint(0.f, 0.f);
	Line offset(oldPos, pos);
	for (auto& iter : walls){
		if (iter.GetAlive()) {
			if (offset.Intersection(iter.GetLine(), iPoint)) {
				alive = false;
				iter.Destroy();
				return;
			}
		}
	}
}

void Bullet::Update(float t, std::vector<Wall>& walls) {
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir *speed* t;				//new position
	PassScreenBorder(pos);
	body.setPosition(pos);
	CheckCollision(walls, oldPos);

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