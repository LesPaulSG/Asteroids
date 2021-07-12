#include "Player.h"

Player::Player(sf::Vector2f pos, float cRotation) :
		Actor(pos, sf::Vector2f(0.f, 0.f), STARSHIP_PATTERN),
		flame(pos, FLAME_PATTERN),
		rotation(cRotation),
		rotSpeed(5.f),
		force(0.f),
		lives(3),
		rDir(RotateDir::STP),
		canMove(true),
		thrustOn(false),
		moved(false)
{
	speed = 100.f;
	RotateUnitVector(dir, cRotation);
}

float Player::GetRotation() const { return rotation; }

int Player::GetLives() const { return lives; }

bool Player::CanMove() const { return canMove; }

void Player::Thrust(bool on){
	thrustOn = on;
	if (thrustOn) {
		RotateUnitVector(dir, rotation);
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
	moved = true;
	body.Move(pos);
}

void Player::Update(float time, std::vector<Actor*>& actors) {
	static float passedTime = 0.f;
	if (canMove) {
		Move(time);
		Rotate(time);
		if (moved) Collision(actors);
	}
	else if ((passedTime += time) >= 1.5f) {
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

void Player::Move(float deltaTime){
	if (force > 0.f) {
		if (thrustOn) RotateUnitVector(dir, rotation);
		else force -= 0.1f * deltaTime;
		pos += dir * force * speed * deltaTime;
		PassScreenBorder(pos);
		body.Move(pos);
		flame.Move(pos);
		moved = true;
	}
}

void Player::Rotate(float deltaTime){
	if (rDir != RotateDir::STP) {
		rotation += rotSpeed * deltaTime * (int)rDir;
		body.Rotate(rotation);
		flame.Rotate(rotation);
		moved = true;
	}
}

bool Player::Collision(const std::vector<Actor*>& actors) {
	static float radSum = 0, dist = 0;
	for (auto iter : actors) {
		if (iter != this) {
			radSum = GetBodyRadius() + iter->GetBodyRadius();
			dist = Line::Distance(pos, iter->GetPos());
			if (radSum >= dist) {
				for (auto& edge : body.GetEdges()) {
					if (iter->DeepCollision(edge.GetLine())) {
						iter->Destroy();
						Destroy();
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Player::SetRotateDir(RotateDir nDir){
	rDir = nDir;
}

void Player::BonusLife(){
	++lives;
	PlaySound(Sound::EXTRA);
}

void Player::Refresh(){
	pos = PLAYER_DEFAULT_POS;
	body.Reset(pos, STARSHIP_PATTERN);
	flame.Rotate(0.f);
	RotateUnitVector(dir, 0.f);
	canMove = true;
	thrustOn = false;
	moved = false;
	rDir = RotateDir::STP;
	rotation = 0.f;
	force = 0.f;
}

void Player::Reset(){
	lives = 3;
	alive = true;
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
