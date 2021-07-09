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
	std::cout << "player constructed\n";
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
	static float old = rotation;
	static Delay del(1.5f);
	if (canMove) {
		pos += dir * force * speed * time;
		rotation += rotSpeed * time * rDir;
		if (force > 0.f)	force -= 0.1f * time;
		PassScreenBorder(pos);
		Collision(actors);
		body.Move(pos);
		flame.Move(pos);
		if (old != rotation) {
			body.Rotate((old - rotation) * -1.f);
			flame.Rotate((old - rotation) * -1.f);
			old = rotation;
			if (thrustOn) SetRotation(rotation);
		}
	}
	else if (del.Wait(time)) {
		Refresh();
		flame.Rotate((old - rotation) * -1.f);
		old = 0.f;
	}
	else {
		if (lives <= 0) {
			static Delay death(1.4f);
			if (death.Wait(time)) {
				alive = false;
			}
		}
		body.Explode();
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
	if (lives < 5) {
		++lives;
		PlaySound(Sound::EXTRA);
	}
}

void Player::Refresh(){
	body = Polygon(PLAYER_DEFAULT_POS, STARSHIP_PATTERN);
	canMove = true;
	thrustOn = false;
	pos = PLAYER_DEFAULT_POS;
	SetRotation(0.f);
	rDir = STP;
	rotation = 0.f;
	force = 0.f;
}
void Player::Draw(sf::RenderWindow& w) const {
	static bool flameDrawed = false;
	Actor::Draw(w);
	if (canMove) {
		if (thrustOn && !flameDrawed) flame.Draw(w);
		flameDrawed = !flameDrawed;
	}
}

void Player::Destroy(){
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
