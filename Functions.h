#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <iostream>
#include <random>

const int HEIGHT = sf::VideoMode::getDesktopMode().height;
const int WIDTH = sf::VideoMode::getDesktopMode().width;

static std::random_device rd;
static std::mt19937_64 gen(rd());

const int BULLETS_MAX_CAPACITY	 = 50;
const int POLYGON_MAX_SIZE		 = 10;
const int ASTEROIDS_MAX_QUANTITY = 30;

const sf::Vector2f PLAYER_DEFAULT_POS(WIDTH / 2, HEIGHT / 2);

const std::vector<sf::Vector2f> STARSHIP_PATTERN{ 
	sf::Vector2f(0, -20),
	sf::Vector2f(10, 10),
	sf::Vector2f(6, 6),
	sf::Vector2f(-6, 6),
	sf::Vector2f(-10, 10) };

const std::vector<sf::Vector2f> BIG_SAUCER_PATTERN{
	sf::Vector2f(0, -20),
	sf::Vector2f(30, 0),
	sf::Vector2f(0, 20),
	sf::Vector2f(-30, 0) };

const std::vector<sf::Vector2f> SMALL_SAUCER_PATTERN{
	sf::Vector2f(0, -10),
	sf::Vector2f(15, 0),
	sf::Vector2f(0, 10),
	sf::Vector2f(-15, 0) };

struct Shot {
	sf::Vector2f from;
	float angle;
	Shot(sf::Vector2f f, float a) : from(f), angle(a) {};
};

enum class Sound{ FIRE, THRUST, BANG_S, BANG_M, BANG_L, SOUC_S, SOUC_B, EXTRA };

void LoadSounds();

void PlaySound(Sound ss);

void LoopSound(Sound ss);

void EndSoundLoop(Sound ss);

bool isPointInRange(float x, float a, float b);

float VectorsModule(const sf::Vector2f& vec);

void RotateVector(sf::Vector2f& vec, float angle);

void RotateVectorAround(sf::Vector2f& vec, const sf::Vector2f& anchor, float angle);

void RotateUnitVector(sf::Vector2f& vec, float angle);

bool PassScreenBorder(sf::Vector2f& vec);