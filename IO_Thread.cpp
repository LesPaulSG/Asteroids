#include "IO_Thread.h"

sf::Vector2i LmbStartPos,    RmbStartPos;			//mouse positions for firing
sf::Vector2i LmbReleasedPos, RmbReleasedPos;		//and creating walls

void input(BulletManager& bm, std::chrono::duration<float>& t, bool& gameOver) {
	//sf::Font font;									//text obj for UI
	//font.loadFromFile("OpenSans-Bold.ttf");			//
	//sf::Text ui("", font, 16);						//
	//ui.setPosition(5.f, 5.f);							//
	//ui.setFillColor(sf::Color::Red);				//

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BulletManager", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);

	std::chrono::duration<float> time;
	auto clock = std::chrono::high_resolution_clock::now();

	sf::Event evt;

	while (true) {
		while (window.pollEvent(evt)) {
			CheckEvent(evt, bm, time.count());
			if (evt.type == sf::Event::Closed) {
				gameOver = true;
				window.close();
			}	else if (evt.type == sf::Event::MouseButtonPressed) {
				if (evt.mouseButton.button == sf::Mouse::Right) {
					RmbStartPos = sf::Mouse::getPosition();											//save coordinates of LMB pressed
				}
			} else if (evt.type == sf::Event::MouseButtonReleased) {
				if (evt.mouseButton.button == sf::Mouse::Left) {
					LmbReleasedPos = sf::Mouse::getPosition();
					sf::Vector2f direction = (sf::Vector2f)LmbReleasedPos - bm.GetPlayer().GetPosition();
					Task task{ TaskType::ADD_BULLET, PackagedTask{bm.GetPlayer().GetPosition(), direction, 10, 10} };
					bm.AddTask(task);
				} else if (evt.mouseButton.button == sf::Mouse::Right) {
					RmbReleasedPos = sf::Mouse::getPosition();
					Task task{ TaskType::ADD_WALL, PackagedTask{sf::Vector2f(RmbStartPos), sf::Vector2f(RmbReleasedPos), true} };
					bm.AddTask(task);
				}
			}
		}

		window.clear();

		{
			std::lock_guard guard(bm.GetBmMutex());
			for (auto& iter : bm.GetWalls()){
				window.draw(iter.GetBody());
			}
			for (auto& iter : bm.GetBullets()){
				window.draw(iter.GetBody());
			}
		}
		window.draw(bm.GetPlayer().GetBody());

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
	case sf::Event::MouseButtonPressed:
		break;
	case sf::Event::MouseButtonReleased:
		break;
	case sf::Event::MouseMoved:
		bm.GetPlayer().Rotate(std::move(sf::Vector2f(sf::Mouse::getPosition())));
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
	}
}