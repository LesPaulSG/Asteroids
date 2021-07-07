#include "IO_Thread.h"
#include "Explosion.h"
#include "Saucer.h"

#include "SFML/Audio.hpp"

std::vector<Explosion> VFX;
std::vector<Polygon> lives;

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver) {
	sf::Font font;
	font.loadFromFile("zig.ttf");
	sf::Text debug("", font, 16);
	debug.setPosition(5.f, 150.f);
	debug.setFillColor(sf::Color::Red);
	sf::Text ui("", font, 30);
	ui.setPosition(5.f, 5.f);
	ui.setFillColor(sf::Color::White);

	for (int i = 0; i < 3; ++i) {
		lives.push_back(Polygon(sf::Vector2f(30 * i + 30, 70), STARSHIP_PATTERN));
	}

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	std::chrono::duration<float> time{ 0 };
	auto clock = std::chrono::high_resolution_clock::now();

	sf::Event evt;

	while (true) {
		while (window.pollEvent(evt)) {
			CheckEvent(evt, bm, time.count());
		}

		if (bm.isExplosions()) {
			VFX.push_back(bm.PopExplosion());
		}
		for (auto iter = VFX.begin(); iter != VFX.end(); ++iter) {
			iter->Update(time.count());
			if (!iter->isAlive()) {
				VFX.erase(iter);
				break;
			}
		}

		window.clear();

		for (auto& iter : VFX) {
			iter.Draw(window);
		}
		for (int i = 0; i < bm.GetPlayerLives(); ++i) {
			lives.at(i).Draw(window);
		}

		bm.Draw(window);

		debug.setString("fps:  " + std::to_string(1.f / time.count())	//GUI FPS
					  + "\npFps: " + std::to_string(1.f / t.count()));	//physics FPS*/
		ui.setString("SCORE: " + std::to_string(bm.GetScore()));

		window.draw(debug);
		window.draw(ui);
		window.display();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			gameOver = true;
			break;
		}

		time  = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
}

void CheckEvent(sf::Event& event, BulletManager& bm, float t){
	switch (event.type){
	case sf::Event::Closed:
		break;
	case sf::Event::KeyPressed:
		KeyboardCheck(event, bm, t);
		break;
	case sf::Event::KeyReleased:
		bm.GetPlayer().SetDir(STP);
		break;
	default:
		break;
	}
}

void KeyboardCheck(sf::Event& event, BulletManager& bm, float t){
	switch (event.key.code) {
	case sf::Keyboard::W:
		bm.GetPlayer().SetDir(BST);
		break;
	case sf::Keyboard::D:
		bm.GetPlayer().SetDir(RGH);
		break;
	case sf::Keyboard::A:
		bm.GetPlayer().SetDir(LFT);
		break;
	case sf::Keyboard::Space:	
		bm.Shoot();
		break;
	}
}