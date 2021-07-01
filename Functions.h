#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include <iostream>

const int BULLETS_MAX_CAPACITY	 = 50;
const int WALLS_MAX_CAPACITY	 = 50;
const int POLYGON_MAX_SIZE		 = 10;
const int ASTEROIDS_MAX_QUANTITY = 30;

const int HEIGHT = sf::VideoMode::getDesktopMode().height;
const int WIDTH  = sf::VideoMode::getDesktopMode().width;

bool isPointInRange(float x, float a, float b);

float VectorsModule(const sf::Vector2f& vec);

void RotateVector(sf::Vector2f& vec, float angle);

void RotateUnitVector(sf::Vector2f& vec, float angle);

void PassScreenBorder(sf::Vector2f& vec);