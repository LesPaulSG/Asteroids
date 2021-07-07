#pragma once
#include "Actor.h"

class Asteroid : public Actor {
private:
	int stage;
	bool alive;
public:
	Asteroid(sf::Vector2f pos_, sf::Vector2f dir_, int stage_ = 3);
	
	int GetStage() const;
	//void foo() override;
};

