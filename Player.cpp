#include "Player.h"

Player::Player(sf::Vector2f pos, float cRotation) :
		Actor(pos, sf::Vector2f(0.f, 0.f), STARSHIP_PATTERN),
		rotation(0.f),
		rotSpeed(5.f),
		force(0.f),
		lives(3),
		canMove(true),
		thrustOn(false),
		flame(pos, FLAME_PATTERN)
{
	speed = 100.f;
	SetRotation(cRotation);
}

void Player::Thrust(bool on){
	thrustOn = on;
	if (thrustOn) {
		SetRotation(rotation);
		force = 1.5f;
		LoopSound(Sound::THRUST);
	}
	else {
		EndSoundLoop(Sound::THRUST);
	}
}

void Player::HyperJump(){
	pos.x = RAND_X(gen);
	pos.y = RAND_Y(gen);
	force = 0.f;
}

void Player::Move(float time, std::vector<Actor*>& actors) {
	static float passedTime = 0.f;
	if (canMove) {
		if (force > 0.f) {
			if (thrustOn) SetRotation(rotation);
			pos += dir * force * speed * time;
			force -= 0.1f * time;
			PassScreenBorder(pos);
			Collision(actors);
			body.Move(pos);
			flame.Move(pos);
		}
		if (rDir != RotateDir::STP) {
			rotation += rotSpeed * time * rDir;
			body.Rotate((rotation) * -1.f);
			flame.Rotate((rotation) * -1.f);
		}
	}
	else if ((passedTime+=time) >= 1.5f) {
		passedTime = 0.f;
		if (lives < 1) {
			alive = false;
			return;
		}
		Refresh();
	}
	else {
		body.Explode(time);
		body.Move(pos);
	}
}

bool Player::Collision(const std::vector<Actor*>& actors){
	if (!canMove) return false;
	static float radSum = 0, dist = 0;
	for (auto iter : actors) {
		if (iter != this) {
			radSum = GetBodyRadius() + iter->GetBodyRadius();
			dist = Line::Distance(pos, iter->GetPos());
			if (radSum >= dist) {
				//add deep collision check?
				iter->Destroy();
				Destroy();
				return true;
			}
		}
	}
	return false;
}

void Player::Rotate(RotateDir nDir){
	rDir = nDir;
}

void Player::SetRotation(float angle){
	float fxNew = sin(rotation);
	float fyNew = -cos(rotation);
	dir.x = fxNew;
	dir.y = fyNew;
}

void Player::BonusLife(){
	++lives;
	PlaySound(Sound::EXTRA);
}

void Player::Refresh(){
	body = Polygon(PLAYER_DEFAULT_POS, STARSHIP_PATTERN);
	flame.Rotate(0.f);
	canMove = true;
	thrustOn = false;
	pos = PLAYER_DEFAULT_POS;
	SetRotation(0.f);
	rDir = STP;
	rotation = 0.f;
	force = 0.f;
}

void Player::Reset(){
	body = Polygon(PLAYER_DEFAULT_POS, STARSHIP_PATTERN);
	pos = PLAYER_DEFAULT_POS;
	SetRotation(0.f);
	dir = sf::Vector2f(0.f, 0.f);
	rotation = 0.f;
	force = 0.f;
	lives = 3;
	alive = true;
	canMove = true;
	thrustOn = false;
	Refresh(); 
}

void Player::Draw(sf::RenderWindow& w) const {
	static bool flameDrawed = false;
	Actor::Draw(w);
	if (canMove) {
		if (thrustOn && !flameDrawed) flame.Draw(w);
		flameDrawed = !flameDrawed;
	}
}

void Player::Destroy(bool playerDestroy){
	--lives;
	EndSoundLoop(Sound::THRUST);
	canMove = false;
}

float Player::GetRotation() const{
	return rotation;
}

int Player::GetLives(){
	return lives;
}

bool Player::CanMove(){
	return canMove;
}
