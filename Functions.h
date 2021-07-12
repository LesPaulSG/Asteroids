#pragma once
#include "Constants.h"

enum class Sound { FIRE, THRUST, BANG_S, BANG_M, BANG_L, SOUC_S, SOUC_B, EXTRA };

struct Shot {
	sf::Vector2f from;
	float angle;
	Shot(sf::Vector2f f, float a) : from(f), angle(a) {}
};

void LoadResources();

void PlaySound(Sound s);
void LoopSound(Sound s);
void EndSoundLoop(Sound s);

const sf::Font& GetFont();

bool isPointInRange(float x, float a, float b);

float VectorsModule(const sf::Vector2f& vec);

void RotateVector(sf::Vector2f& vec, float angle, const sf::Vector2f& anchor = sf::Vector2f(0.f, 0.f));
void RotateUnitVector(sf::Vector2f& vec, float angle);

bool PassScreenBorder(sf::Vector2f& vec);

void FormatText(sf::Text& txt);
void TextToCenter(sf::Text& txt);
