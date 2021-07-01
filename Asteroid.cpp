#include "Asteroid.h"

Asteroid::Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_)
	: pos(pos_),
	dir(dir_),
	speed(50.f),
	stage(stage_),
	alive(true) {
	radius = stage * 15;
	body.setFillColor(sf::Color::Red);
	body.setRadius(radius);
	body.setOrigin(sf::Vector2f(body.getRadius() / 2, body.getRadius() / 2));
	std::cout << "ya_rodilsyia " << stage << ' ' << pos.x << ' ' << pos.y << std::endl;
}

Asteroid::~Asteroid(){
	if (--stage > 0) {
		//
	}
}

void Asteroid::Move(float time){
	sf::Vector2f oldPos = pos;			//position before update
	pos += dir * speed * time;				//new position
	PassScreenBorder(pos);
	body.setPosition(pos);
	//check collision
}

void Asteroid::CheckCollision(std::vector<Asteroid>& asteroids){

}

void Asteroid::Draw(sf::RenderWindow& w){
	w.draw(body);
}

void Asteroid::Destroy(){
	alive = false;
}

const sf::Vector2f& Asteroid::GetPos() const{
	return pos;
}

float Asteroid::GetRadius() const{
	return radius;
}

bool Asteroid::GetAlive() const {
	return alive;
}

int Asteroid::GetStage() const {
	return stage;
}
