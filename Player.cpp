#include "Player.h"

Player::Player(sf::Vector2f pos, float cRotation) 
	: pos(std::move(pos)),
	rotation(0.f),
	speed(150.f),
	rotSpeed(5.f),
	radius(1.f),
	body(pos, { sf::Vector2f(0, -20), sf::Vector2f(10, 10), sf::Vector2f(0, 5), sf::Vector2f(-10, 10) }) {
	forwardVector = sf::Vector2f(0.f, -1.f);
	rightVector   = sf::Vector2f(1.f, 0.f);

	Rotate(cRotation);
}

void Player::CheckCollision(float time, const sf::Vector2f& oldPos, std::vector<Wall>& walls){
	sf::Vector2f iPoint(0.f, 0.f);
	Line offset(oldPos, pos);
	int i = 0;
	for (auto& iter : walls) {
		if (iter.GetAlive()) {
			if (iter.GetLine().CircleIntersection(pos, radius, iPoint)) {
				Collision(time, iPoint, oldPos, iter);
				//break;
			}
		}
	}
}

void Player::Collision(float time, const sf::Vector2f& iPoint, const sf::Vector2f& oldPos, const Wall& wall){
	float angle = wall.GetLine().AngleOfIntersec(Line(oldPos, pos));
	sf::Vector2f tmpDir;
	if (angle >= 1.5708f) {
		tmpDir = wall.GetLine().pointA;// -forwardVector;// *radius;
	} else {
		tmpDir = wall.GetLine().pointB;// +forwardVector;// *radius;
	}
	
	if (!wall.GetLine().isPointRight(pos)) {
		pos -= (tmpDir - forwardVector) * time;// +forwardVector;
	} else {
		pos += (tmpDir + forwardVector) * time;// -forwardVector;
	}
}

void Player::Update(float time) {
	float old = rotation;
	if (Move(time)) {
		PassScreenBorder(pos);
		body.Move(pos);
		if(old!=rotation)
			body.Rotate((old-rotation)*-1.f);
	}
}

void Player::Rotate(float angle){
	float fxNew = sin(rotation);
	float fyNew = -cos(rotation);
	forwardVector.x = fxNew;
	forwardVector.y = fyNew;
	float rxNew = cos(rotation);
	float ryNew = sin(rotation);
	rightVector.x = rxNew;
	rightVector.y = ryNew;
}

bool Player::Move(float time){
	switch (dir){
	case STP:
		return false;
	case FWD:
		pos += forwardVector * time * speed;
		return true;
	case BWD:
		pos -= forwardVector * time * speed;
		return true;
	case RGH:
		rotation += rotSpeed * time;
		Rotate(rotation);
		return true;
	case LFT:
		rotation -= rotSpeed * time;
		Rotate(rotation);
		return true;
	default:
		return false;
	}
}

const sf::Vector2f& Player::GetPosition() const {return pos;}

const sf::Vector2f& Player::GetForwardVector() const {return forwardVector;}

float Player::GetRotation() const{
	return rotation;
}

void Player::SetDir(MoveDir nDir){
	dir = nDir;
}

void Player::Draw(sf::RenderWindow& w){
	body.Draw(w);
}
