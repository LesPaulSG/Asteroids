#include "Player.h"

Player::Player(sf::Vector2f pos, float cRotation) 
	: pos(std::move(pos)),
	rotation(0.f),
	speed(100.f),
	rotSpeed(0.000001f),
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
	//std::cout << rotation << std::endl;
	//Move(time/10);
	//pos += tmpDir * time * speed;
	//ChangeDirection(angle, wall.isPointRight(oldPos));
	//pos = iPoint + forwardVector * 0.00001f;		//update positon to intersection point + small distance
	//pos = oldPos;
	//body.setPosition(pos);
	//std::cout << pos.x << ' ' << pos.y << std::endl;
	//speed *= 0.95f;
}

void Player::Update(float time, std::vector<Wall>& walls) {
	const sf::Vector2f oldPos = pos;
	if (Move(time)) {
		//CheckCollision(time, oldPos, walls);
		PassScreenBorder(pos);
		body.Move(pos);
	}
	//CheckCollision(oldPos, walls);
}

void Player::Rotate(float angle){
	std::cout << angle << std::endl;
	if (angle > 6.28319) {
		angle = 0;
	}
	else if (angle < 0) {
		angle = 6.28319;
	}
	rotation = angle - 1.5708f;

	float fxNew = sin(rotation);
	float fyNew = -cos(rotation);
	forwardVector.x = fxNew;
	forwardVector.y = fyNew;
	//RotateUnitVector(forwardVector, angle);
	//RotateUnitVector(rightVector, angle);
	float rxNew = cos(rotation);
	float ryNew = sin(rotation);
	rightVector.x = rxNew;
	rightVector.y = ryNew;
}

/*void Player::Rotate(const sf::Vector2f& mousePos){
	static Line horizontalLine(sf::Vector2f(0, 0), sf::Vector2f(1920, 0));
	Line posToMouse(pos, mousePos);
	float angle = horizontalLine.AngleOfIntersec(posToMouse);
	angle *= -1.f;
	if (mousePos.y < pos.y) {
		angle += 3.14159f;
	}
	Rotate(angle);
}*/

bool Player::Move(float time){
	float oldRot = rotation;
	switch (dir){
	case STP:
		return false;
	case FWD:
		pos += forwardVector * time * speed;
		return true;
		break;
	case BWD:
		pos -= forwardVector * time * speed;
		return true;
		break;
	case RGH:
		//pos += rightVector * time * speed;
		Rotate(rotation + rotSpeed * time);
		body.Rotate(rotation);
		return true;
		break;
	case LFT:
		//pos -= rightVector * time * speed;
		Rotate(rotation - rotSpeed * time);
		body.Rotate(rotation);
		return true;
		break;
	default:
		return false;
		break;
	}
}

//const sf::CircleShape& Player::GetBody() const {return body;}

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
