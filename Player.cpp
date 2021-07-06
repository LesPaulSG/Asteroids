#include "Player.h"

Player::Player(sf::Vector2f pos, float cRotation) 
	: pos(std::move(pos)),
	rotation(0.f),
	speed(100.f),
	rotSpeed(5.f),
	radius(20.f),
	force(0.f),
	lives(3),
	body(pos, STARSHIP_PATTERN) {
	forwardVector = sf::Vector2f(0.f, -1.f);
	rightVector   = sf::Vector2f(1.f, 0.f);

	Rotate(cRotation);
}

void Player::CheckCollision(float time, std::vector<Asteroid>& asteroids){
	float radSum = 0, dist = 0;
	for (auto& iter : asteroids) {
		radSum = radius + iter.GetRadius();
		dist = Line(pos, iter.GetPos()).lenght;
		if (dist <= radSum) {
			for (auto& b : body.getEdges()) {
				if (iter.isCollision(b.GetLine())) {
					--lives;
					Refresh();
					return;
				}
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

void Player::Update(float time, std::vector<Asteroid>& asteroids) {
	float old = rotation;
	Move(time);
	pos += forwardVector * force * speed * time;
	if(force > 0.f)	force -= 0.1f * time;
	PassScreenBorder(pos);
	CheckCollision(time, asteroids);
	body.Move(pos);
	if(old!=rotation)
		body.Rotate((old-rotation)*-1.f);
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
	case BST:
		Rotate(rotation);
		force = std::min((force + time * speed), 1.f);
		PlaySound(Sound::THRUST);
		return true;
	case RGH:
		rotation += rotSpeed * time;
		//Rotate(rotation);
		return true;
	case LFT:
		rotation -= rotSpeed * time;
		//Rotate(rotation);
		return true;
	default:
		return false;
	}
}

void Player::BonusLife(){
	if (lives < 3) {
		++lives;
		PlaySound(Sound::EXTRA);
	}
}

void Player::Refresh(){
	pos = PLAYER_DEFAULT_POS;
	Rotate(1.57);
}

const sf::Vector2f& Player::GetPosition() const {return pos;}

const sf::Vector2f& Player::GetForwardVector() const {return forwardVector;}

float Player::GetRotation() const{
	return rotation;
}

int Player::GetLives(){
	return lives;
}

void Player::SetDir(MoveDir nDir){
	dir = nDir;
}

void Player::Draw(sf::RenderWindow& w){
	body.Draw(w);
}
