#include "IO_Thread.h"
#include "Explosion.h"
#include "Saucer.h"

#include "SFML/Audio.hpp"

std::vector<Explosion> VFX;
std::vector<Polygon> lives;

BulletManager* BM;
bool* GAME_OVER;
sf::RenderWindow* W;

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver) {
	sf::Font font;
	font.loadFromFile("vector.ttf");
	sf::Text debug("", font, 16);
	debug.setPosition(5.f, 150.f);
	debug.setFillColor(sf::Color::Red);
	sf::Text ui("", font, 30);
	ui.setOutlineColor(sf::Color::White);
	ui.setOutlineThickness(0.5);
	ui.setPosition(5.f, 5.f);
	ui.setFillColor(sf::Color::White);

	for (int i = 0; i < 5; ++i) {
		lives.push_back(Polygon(sf::Vector2f(30 * i + 30, 70), STARSHIP_PATTERN));
	}

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	std::chrono::duration<float> time{ 0 };
	auto clock = std::chrono::high_resolution_clock::now();

	bool playerDead = false;

	BM = &bm;
	GAME_OVER = &gameOver;
	W = &window;

	while (W->isOpen()) {
		Game(time.count());

		time = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();

	}
}

void Start(float deltaTime)
{
}

void Game(float deltaTime)
{
	sf::Event evt;

	while (true) {
		while (W->pollEvent(evt)) {
			if (BM->GetPlayer().CanMove())
				CheckEvent(evt, *BM, deltaTime);
		}

		if (BM->isExplosions()) {
			VFX.push_back(BM->PopExplosion());
		}
		for (auto iter = VFX.begin(); iter != VFX.end(); ++iter) {
			iter->Update(deltaTime);
			if (!iter->isAlive()) {
				VFX.erase(iter);
				break;
			}
		}

		W->clear();

		for (auto& iter : VFX) {
			iter.Draw(*W);
		}
		for (int i = 0; i < BM->GetPlayerLives(); ++i) {
			lives.at(i).Draw(*W);
		}

		BM->Draw(*W);

		//debug.setString("fps:  " + std::to_string(1.f / time.count())	//GUI FPS
		//	+ "\npFps: " + std::to_string(1.f / t.count()));	//physics FPS*/
		//ui.setString(std::to_string(BM->GetScore()));

		//W->draw(debug);
		//W->draw(ui);
		W->display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			*GAME_OVER = true;
			W->close();
		}
	}
}

void NewTopScore(float deltaTime)
{
}

void LeaderBoard(float deltaTime)
{
}

void CheckEvent(sf::Event& event, BulletManager& bm, float t){
	switch (event.type){
	case sf::Event::Closed:
		break;
	case sf::Event::KeyPressed:
		KeyboardPressCheck(event, bm, t);
		break;
	case sf::Event::KeyReleased:
		KeyboardReleaseCheck(event, bm, t);
		break;
	}
}

void KeyboardPressCheck(sf::Event& event, BulletManager& bm, float t){
	switch (event.key.code) {
	case sf::Keyboard::W:
		bm.GetPlayer().Thrust(true);
		break;
	case sf::Keyboard::D:
		bm.GetPlayer().Rotate(RGH);
		break;
	case sf::Keyboard::A:
		bm.GetPlayer().Rotate(LFT);
		break;
	}
}

void KeyboardReleaseCheck(sf::Event& event, BulletManager& bm, float t) {
	switch (event.key.code) {
	case sf::Keyboard::W:
		bm.GetPlayer().Thrust(false);
		break;
	case sf::Keyboard::D:
		bm.GetPlayer().Rotate(STP);
		break;
	case sf::Keyboard::A:
		bm.GetPlayer().Rotate(STP);
		break;
	case sf::Keyboard::V:
		bm.GetPlayer().HyperJump();
		break;
	case sf::Keyboard::Space:
		bm.Shoot();
		break;
	}
}