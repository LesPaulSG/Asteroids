#include "Player.h"

std::uniform_int_distribution<int> x(50, WIDTH  - 50);
std::uniform_int_distribution<int> y(50, HEIGHT - 50);

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
	dir = sf::Vector2f(0.f, -1.f);
	SetRotation(cRotation);
}

bool Player::Collision(std::vector<Actor*>& actors){
	if (Actor::Collision(actors)) {
		--lives;
		return true;
	}
	return false;
}

void Player::Thrust(bool on){
	if (canMove) {
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
}

void Player::HyperJump(){
	pos.x = x(gen);
	pos.y = y(gen);
	force = 0.f;
}

void Player::Move(float time, std::vector<Actor*>& actors) {
	static float old = rotation;
	if (canMove) {
		pos += dir * force * speed * time;
		rotation += rotSpeed * time * rDir;
		if (force > 0.f)	force -= 0.1f * time;
		PassScreenBorder(pos);
		canMove = !Collision(actors);
		body.Move(pos);
		flame.Move(pos);
		if (old != rotation) {
			body.Rotate((old - rotation) * -1.f);
			flame.Rotate((old - rotation) * -1.f);
			old = rotation;
			if (thrustOn) SetRotation(rotation);
		}
	}
	else if (Delay(time, 1.5f)) {
		std::cout << "delay passed\n";
		Refresh();
	}
}

void Player::Rotate(RotateDir nDir){
	if(canMove)	rDir = nDir;
}

void Player::SetRotation(float angle){
	float fxNew = sin(rotation);
	float fyNew = -cos(rotation);
	dir.x = fxNew;
	dir.y = fyNew;
}

void Player::BonusLife(){
	if (lives < 3) {
		++lives;
		PlaySound(Sound::EXTRA);
	}
}

void Player::Refresh(){
	canMove = true;
	pos = PLAYER_DEFAULT_POS;
	SetRotation(0.f);
	rDir = STP;
	rotation = 0.f;
	force = 0.f;
}
void Player::Draw(sf::RenderWindow& w){
	Actor::Draw(w);
	if (thrustOn) flame.Draw(w);
}

void Player::Destroy(){
	--lives;
	canMove = false;
}

float Player::GetRotation() const{
	return rotation;
}

int Player::GetLives(){
	return lives;
}