#include "IO_Thread.h"
#include "Explosion.h"

sf::Vector2i LmbStartPos,    RmbStartPos;			//mouse positions for firing
sf::Vector2i LmbReleasedPos, RmbReleasedPos;		//and creating walls

std::vector<Explosion> VFX;

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver) {
	sf::Font font;									//text obj for UI
	font.loadFromFile("OpenSans-Bold.ttf");			//
	sf::Text ui("", font, 16);						//
	ui.setPosition(5.f, 5.f);						//
	ui.setFillColor(sf::Color::Red);				//

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BulletManager", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

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

		bm.Draw(window);

		//crushing on the startup
		/*ui.setString("bullest:  " + std::to_string(bm.GetBullets().size())	//bullets quantity
					+ "\nwalls:     " + std::to_string(bm.GetWalls().size())//walls quantity
					+ "\nfps:         " + std::to_string(1.f / time.count())//GUI FPS
					+ "\npFps:       " + std::to_string(1.f / t.count()));	//physics FPS*/

		//window.draw(ui);
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
		bm.GetPlayer().SetDir(FWD);
		break;
	case sf::Keyboard::S:
		bm.GetPlayer().SetDir(BWD);
		break;
	case sf::Keyboard::D:
		bm.GetPlayer().SetDir(RGH);
		break;
	case sf::Keyboard::A:
		bm.GetPlayer().SetDir(LFT);
		break;
	case sf::Keyboard::Space:
		Task task{ TaskType::ADD_BULLET, PackagedTask{bm.GetPlayer().GetPosition(), bm.GetPlayer().GetRotation(), 1000, 10} };
		bm.AddTask(task);
		break;
	}
}