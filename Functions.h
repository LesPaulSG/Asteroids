#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <iostream>
#include <random>

const int HEIGHT = sf::VideoMode::getDesktopMode().height;
const int WIDTH = sf::VideoMode::getDesktopMode().width;

static std::random_device rd;
static std::mt19937_64 gen(rd());

const std::uniform_int_distribution<int> randomBool(0, 1);

const int BULLETS_MAX_CAPACITY	 = 50;
const int POLYGON_MAX_SIZE		 = 10;
const int ASTEROIDS_MAX_QUANTITY = 30;

const sf::Vector2f PLAYER_DEFAULT_POS(WIDTH / 2, HEIGHT / 2);

struct VectorPair {
	sf::Vector2f f;
	sf::Vector2f s;
	VectorPair(sf::Vector2f f_, sf::Vector2f s_) : f(f_), s(s_) {}
};

const std::vector<VectorPair> STARSHIP_PATTERN{
	VectorPair(sf::Vector2f(  0.f, -20.f), sf::Vector2f( 10.f,  10.f)),
	VectorPair(sf::Vector2f( 10.f,  10.f), sf::Vector2f(  6.f,   6.f)),
	VectorPair(sf::Vector2f(  6.f,   6.f), sf::Vector2f( -6.f,   6.f)),
	VectorPair(sf::Vector2f( -6.f,   6.f), sf::Vector2f(-10.f,  10.f)),
	VectorPair(sf::Vector2f(-10.f,  10.f), sf::Vector2f(  0.f, -20.f)) };

const std::vector<VectorPair> FLAME_PATTERN{
	VectorPair(sf::Vector2f( -5.f,	 6.f), sf::Vector2f(  5.f,   6.f)),
	VectorPair(sf::Vector2f(  5.f,   6.f), sf::Vector2f(  0.f,  15.f)),
	VectorPair(sf::Vector2f(  0.f,  15.f), sf::Vector2f( -5.f,   6.f)) };

const std::vector<VectorPair> BIG_SAUCER_PATTERN{
	VectorPair(sf::Vector2f( -9.f,  -19.5f),	sf::Vector2f(  9.f, -19.5f)),
	VectorPair(sf::Vector2f(  9.f,  -19.5f),	sf::Vector2f( 15.f,  -7.5f)),
	VectorPair(sf::Vector2f( 15.f,   -7.5f),	sf::Vector2f(-15.f,  -7.5f)),
	VectorPair(sf::Vector2f(-15.f,   -7.5f),	sf::Vector2f( -9.f, -19.5f)),
	VectorPair(sf::Vector2f(-15.f,   -7.5f),	sf::Vector2f(-30.f,   4.5f)),
	VectorPair(sf::Vector2f(-30.f,    4.5f),	sf::Vector2f( 30.f,   4.5f)),
	VectorPair(sf::Vector2f( 30.f,    4.5f),	sf::Vector2f( 15.f,  -7.5f)),
	VectorPair(sf::Vector2f( 30.f,    4.5f),	sf::Vector2f( 18.f,  13.5f)),
	VectorPair(sf::Vector2f( 18.f,   13.5f),	sf::Vector2f(-18.f,  13.5f)),
	VectorPair(sf::Vector2f(-18.f,   13.5f),	sf::Vector2f(-30.f,   4.5f)) };

const std::vector<VectorPair> SMALL_SAUCER_PATTERN{
	VectorPair(sf::Vector2f(-6.f,  -13.f),	sf::Vector2f(  6.f, -13.f)),
	VectorPair(sf::Vector2f( 6.f,  -13.f),	sf::Vector2f( 10.f,  -5.f)),
	VectorPair(sf::Vector2f( 10.f,  -5.f),	sf::Vector2f(-10.f,  -5.f)),
	VectorPair(sf::Vector2f(-10.f,  -5.f),	sf::Vector2f( -6.f, -13.f)),
	VectorPair(sf::Vector2f(-10.f,  -5.f),	sf::Vector2f(-20.f,   3.f)),
	VectorPair(sf::Vector2f(-20.f,   3.f),	sf::Vector2f( 20.f,   3.f)),
	VectorPair(sf::Vector2f( 20.f,   3.f),	sf::Vector2f( 10.f,  -5.f)),
	VectorPair(sf::Vector2f( 20.f,   3.f),	sf::Vector2f( 12.f,   9.f)),
	VectorPair(sf::Vector2f( 12.f,   9.f),	sf::Vector2f(-12.f,   9.f)),
	VectorPair(sf::Vector2f(-12.f,   9.f),	sf::Vector2f(-20.f,   3.f)) };

struct Shot {
	sf::Vector2f from;
	float angle;
	Shot(sf::Vector2f f, float a) : from(f), angle(a) {}
};

enum class Sound{ FIRE, THRUST, BANG_S, BANG_M, BANG_L, SOUC_S, SOUC_B, EXTRA };

void LoadSounds();

void PlaySound(Sound ss);

void LoopSound(Sound ss);

void EndSoundLoop(Sound ss);

sf::Font& GetFont();

bool isPointInRange(float x, float a, float b);

float VectorsModule(const sf::Vector2f& vec);

void RotateVector(sf::Vector2f& vec, float angle);

void RotateVectorAround(sf::Vector2f& vec, const sf::Vector2f& anchor, float angle);

void RotateUnitVector(sf::Vector2f& vec, float angle);

bool PassScreenBorder(sf::Vector2f& vec);

class Delay {
	float timePassed;
	float waitTime;
public:
	Delay(float wT) : timePassed(0.f), waitTime(wT) {}
	bool Wait(float deltaTime) {
		timePassed += deltaTime;
		if (timePassed > waitTime) {
			timePassed = 0.f;
			return true;
		}
		return false;
	}
};

//bool Delay(float deltaTime, const float waitTime); //maybe functional object will be more suitableb