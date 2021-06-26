#include "Bullet.h"

#include <iostream>

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f dir, float speed, float lifeTime)
		: pos(std::move(pos)),
		dir(std::move(dir)),
		speed(speed),
		time(0.f),
		lifeTime(lifeTime),
		alive(true) {
	this->dir.x = 0.f;
	this->dir.y = -1.f;
	RotateUnitVector(this->dir, Line(pos, dir).rotation);
	body.setPosition(pos);
	body.setOrigin(sf::Vector2f(body.getOrigin().x + 10.f, body.getOrigin().y + 10.f));
	body.setRadius(5.f);
	body.setFillColor(sf::Color::Red);
}

const sf::CircleShape& Bullet::GetBody() const {return body;}

bool Bullet::GetAlive() const {return alive;}

void Bullet::CheckCollision(std::vector<Wall>& walls, const sf::Vector2f& oldPos) {
	sf::Vector2f iPoint(0.f, 0.f);
	Line offset(oldPos, pos);
	for (auto& iter : walls){
		if (iter.GetAlive()) {
			if (offset.Intersection(iter.GetLine(), iPoint)) {
				Collision(iPoint, oldPos, iter.GetLine());
				if (iter.GetDestructable()) {
					iter.Destroy();
				}
				CheckCollision(walls, oldPos);
				return;
			}
		}
	}
}

void Bullet::Update(float t, std::vector<Wall>& walls) {
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir * speed * t;				//new position
	body.setPosition(pos);

	CheckCollision(walls, oldPos);

	speed -= t/30.f;					//braking over time
	time  += t;
		
	LifeCheck();
}

void Bullet::Collision(const sf::Vector2f& iPoint, const sf::Vector2f& oldPos, const Line& wall) {
	float angle = wall.AngleOfIntersec(Line(oldPos, pos));
	ChangeDirection(angle, wall.isPointRight(pos));
	pos = iPoint + dir;// *0.00001f;		//update positon to intersection point + small distance
	body.setPosition(pos);
	 speed *= 0.95f;
}

void Bullet::ChangeDirection(float beta, bool right) {
	float rot = Line(pos, dir).rotation;
	float alpha = 1.5708f - beta;				//calculate alpha angle	
	float angle = 3.14159f + (2.0f * alpha);	//rotation angle
	//float angle = rot - (alpha * 2.f);
	if (!right) {								//here is bug
		angle *= -1.f;
	}
	RotateUnitVector(dir, angle);
}

void Bullet::LifeCheck(){
	if (time >= lifeTime) {
		alive = false;
	} else if (speed <= 0.f) {
		alive = false;
	}
}