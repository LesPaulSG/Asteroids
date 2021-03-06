#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(Shot sho, bool playerShooted) :
		body(1.5f),
		pos(std::move(sho.from)),
		speed(600.f),
		time(0.f),
		lifeTime(1.f),
		alive(true),
		playerOwned(playerShooted)
{
	RotateUnitVector(this->dir, sho.angle);
	body.setFillColor(sf::Color::White);
}

const sf::CircleShape& Bullet::GetBody() const {return body;}

bool Bullet::isAlive() const {return alive;}

bool Bullet::isPlayerOwned() const {return playerOwned;}

void Bullet::CheckCollision(std::vector<Actor*>& actors) {
	sf::Vector2f oldPos = pos;	//position before update
	static float dist = 0;
	Line offset(oldPos, pos);
	for (auto& iter : actors){
		dist = Line::Distance(pos, iter->GetPos());
		if (dist <= iter->GetRadius()) {
			if (iter->DeepCollision(offset)) {
				iter->Destroy(playerOwned);
				alive = false;
				return;
			}
		}
	}
	oldPos = pos;
}

void Bullet::Update(float t, std::vector<Actor*>& actors) {
	pos += dir * speed * t;				//new position
	if (isPassingHorBrd(pos) || isPassingVerBrd(pos)) {
		alive = false;
		return;
	}
	body.setPosition(pos);
	CheckCollision(actors);

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