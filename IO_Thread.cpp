#include "IO_Thread.h"
#include "Saucer.h"

#include "SFML/Audio.hpp"
#include <fstream>

sf::RectangleShape f;
sf::RectangleShape s;
sf::RectangleShape t;

IoManager::IoManager(BulletManager& bm_, bool& gameOver_) :
	bm(bm_),
	gameOver(gameOver_),
	deltaTime(0),
	clock(std::chrono::high_resolution_clock::now()),
	currentState(GameState::START),
	pressAnyKey("Press any key", GetFont(), 30),
	score("0", GetFont(), 30),
	enterInitials("Your score is one of the ten best\nPlease enter your initials\nPush rotate to select letter\nPush hyperspace when letter is correct", GetFont(), 30),
	initials("A", GetFont(), 60),
	leaderBoard("High scores", GetFont(), 30),
	playerDead(false),
	activeSymbol(65)
{
	pressAnyKey.setOutlineColor(sf::Color::White);
	pressAnyKey.setOutlineThickness(0.5);
	pressAnyKey.setPosition(300.f, 300.f);
	pressAnyKey.setFillColor(sf::Color::White);

	score.setOutlineColor(sf::Color::White);
	score.setOutlineThickness(0.5);
	score.setPosition(5.f, 5.f);
	score.setFillColor(sf::Color::White);

	enterInitials.setOutlineColor(sf::Color::White);
	enterInitials.setOutlineThickness(0.5);
	enterInitials.setPosition(300.f, 300.f);
	enterInitials.setFillColor(sf::Color::White);

	initials.setOutlineColor(sf::Color::White);
	initials.setOutlineThickness(0.5);
	initials.setPosition(750.f, 840.f);
	initials.setFillColor(sf::Color::White);
	initials.setLetterSpacing(5.f);

	leaderBoard.setOutlineColor(sf::Color::White);
	leaderBoard.setOutlineThickness(0.5);
	leaderBoard.setPosition(900.f, 250.f);
	leaderBoard.setFillColor(sf::Color::White);

	//leaders.reserve(10);
	lives.reserve(5);
	VFX.reserve(45);
	for (int i = 0; i < 5; ++i) {
		lives.push_back(Polygon(sf::Vector2f(30 * i + 30, 70), STARSHIP_PATTERN));
	}
	for (int i = 0; i < 3; ++i) {
		cursors.push_back(sf::RectangleShape(sf::Vector2f(1, 50)));
		cursors.at(i).setRotation(90);
		cursors.at(i).setFillColor(sf::Color::White);
		cursors.at(i).setPosition(sf::Vector2f(800 + (initials.getCharacterSize()+initials.getLetterSpacing()) * i * 1.65, 900));
	}

	clock = std::chrono::high_resolution_clock::now();
}

void IoManager::Update(){
	w = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Asteroids", sf::Style::Fullscreen);
	w->setMouseCursorVisible(false);

	while (w->isOpen()) {
		while (w->pollEvent(evt)) {
			CheckEvent();
		}

		switch (currentState){
		case START:
			Start();
			break;
		case IN_GAME:
			Game();
			break;
		case GAME_OVER:
			GameOver();
			break;
		case NEW_TOP_SCORE:
			NewTopScore();
			break;
		case LEADERBORD:
			LeaderBoard();
			break;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			gameOver = true;
			w->close();
		}

		deltaTime = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}
}

void IoManager::Start() {
	static float anim = 0.f;
	anim += deltaTime.count();
	w->clear();
	bm.Draw(*w);
	if (anim > 0.5f) {
		w->draw(pressAnyKey);
		if (anim > 1.f) anim = 0.f;
	}
	w->display();
}

void IoManager::Game(){
	if (!bm.GetPlayer().isAlive()) {
		currentState = GAME_OVER;
		LoadLeaderBoard();
		return;
	}

	if (bm.isExplosions()) {
		VFX.push_back(bm.PopExplosion());
	}
	for (auto iter = VFX.begin(); iter != VFX.end(); ++iter) {
		iter->Update(deltaTime.count());
		if (!iter->isAlive()) {
			VFX.erase(iter);
			break;
		}
	}

	w->clear();

	for (auto& iter : VFX) {
		iter.Draw(*w);
	}
	for (int i = 0; i < bm.GetPlayerLives(); ++i) {
		lives.at(i).Draw(*w);
	}

	bm.Draw(*w);

	score.setString(std::to_string(bm.GetScore()));

	w->draw(score);
	w->display();
}

void IoManager::GameOver() {
	static float passed = 0;
	passed += deltaTime.count();
	if (passed > 3) {
		if (leaders.size() < 10) {
			currentState = NEW_TOP_SCORE;
			return;
		}
		for (auto& iter : leaders) {
			if (bm.GetScore() > iter.first) {
				currentState = NEW_TOP_SCORE;
				return;
			}
		}
		currentState = LEADERBORD;
		return;
	}
	pressAnyKey.setString("game over");
	pressAnyKey.setPosition(sf::Vector2f(900, 600));
	w->clear();
	w->draw(pressAnyKey);
	w->display();
}

void IoManager::NewTopScore(){
	w->clear();
	w->draw(score);
	w->draw(enterInitials);
	w->draw(initials);
	for (int i = initials.getString().getSize(); i < 3; ++i) {
		w->draw(cursors.at(i));
	}
	w->display();
}

void IoManager::LeaderBoard(){
	w->clear();
	w->draw(score);
	w->draw(leaderBoard);
	w->display();
}

void IoManager::LoadLeaderBoard(){
	std::ifstream fin("leaders.txt");
	int size, score;
	std::string name;
	fin >> size;
	for (int i = 0; i < size; ++i) {
		fin >> name >> score;
		leaders.push_back(std::make_pair(score, name));
	} 
	fin.close();
	int counter = 0;
	std::sort(leaders.begin(), leaders.end(), []
											(std::pair<int, std::string> f, std::pair<int, std::string> s)
											{return f.first > s.first; });
}

void IoManager::CheckEvent() {
	switch (evt.type) {
	case sf::Event::Closed:
		break;
	case sf::Event::KeyPressed:
		KeyboardPressCheck();
		break;
	case sf::Event::KeyReleased:
		KeyboardReleaseCheck();
		break;
	}
}

void IoManager::KeyboardPressCheck() {
	switch (currentState)	{
	case START:
		currentState = IN_GAME;
		bm.StartGame();
		return;
	case IN_GAME:
		if (bm.GetPlayer().CanMove()) {
			switch (evt.key.code) {
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
		break;
	case LEADERBORD:
		currentState = IN_GAME;
		bm.StartGame();
		return;
	}	
}

void IoManager::KeyboardReleaseCheck() {
	static std::string l_initials;
	switch (currentState) {
	case IN_GAME:
		if (bm.GetPlayer().CanMove()) {
			switch (evt.key.code) {
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
		break;
	case NEW_TOP_SCORE:
		switch (evt.key.code) {
		case sf::Keyboard::V:
			l_initials += activeSymbol;
			initials.setString(l_initials + 'A');
			activeSymbol = 65;
			break;
		case sf::Keyboard::D:
			if ((int)activeSymbol < 90) ++activeSymbol;
			initials.setString(l_initials + activeSymbol);
			break;
		case sf::Keyboard::A:			
			if ((int)activeSymbol > 65) --activeSymbol;
			initials.setString(l_initials + activeSymbol);
			break;
		}
		if (l_initials.size() >= 3) {
			leaders.push_back(std::make_pair(bm.GetScore(), l_initials));
			std::sort(leaders.begin(), leaders.end(), []
			(std::pair<int, std::string> f, std::pair<int, std::string> s)
				{return f.first > s.first; });
			currentState = LEADERBORD;
			int counter = 0;
			for (auto& iter : leaders) {
				++counter;
				leaderBoard.setString(leaderBoard.getString() + "\n" + std::to_string(counter) + ". " + iter.second + " - " + std::to_string(iter.first));
			}
			////////////////////////////
			std::ofstream fout("leaders.txt");
			fout << leaders.size() << std::endl;
			for (auto& iter : leaders) {
				fout << iter.second << " " << iter.first << std::endl;
			}
			fout.close();
			////////////////////////////
			return;
		}
		break;
	}
}