#include "IO_Thread.h"
#include "Saucer.h"

#include "SFML/Audio.hpp"
#include <fstream>

IoManager::IoManager(BulletManager& bm_, bool& gameOver_) :
		bm(bm_),
		gameOver(gameOver_),
		deltaTime(0),
		clock(std::chrono::high_resolution_clock::now()),
		currentState(GameState::START),
		score("0", GetFont()),
		activeText(S_PRESS_ANY_KEY, GetFont()),
		initials("___", GetFont(), 60),
		playerDead(false)
{
	score.setOutlineColor(sf::Color::White);
	score.setOutlineThickness(0.5);
	score.setPosition(5.f, 5.f);
	score.setFillColor(sf::Color::White);

	activeText.setOutlineColor(sf::Color::White);
	activeText.setOutlineThickness(0.5);
	activeText.setPosition(WIDTH/2 - activeText.getGlobalBounds().width/2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
	activeText.setFillColor(sf::Color::White);

	initials.setOutlineColor(sf::Color::White);
	initials.setOutlineThickness(0.5);
	initials.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT * 0.7f - activeText.getGlobalBounds().height / 2);
	initials.setFillColor(sf::Color::White);
	initials.setLetterSpacing(5.f);

	//leaders.reserve(10);
	lives.reserve(5);
	VFX.reserve(45);
	for (int i = 0; i < 5; ++i) {
		lives.push_back(Polygon(sf::Vector2f(30 * i + 30, 70), STARSHIP_PATTERN));
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
		w->draw(activeText);
		if (anim > 1.f) anim = 0.f;
	}
	w->display();
}

void IoManager::Game(){
	if (!bm.GetPlayer().isAlive()) {
		static Delay del(1.5f);
		if (del.Wait(deltaTime.count())) {
			bm.Clear();
			ChangeOnOver();
			LoadLeaderBoard();
			return;
		}
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
			ChangeOnInitials();
			return;
		}
		for (auto& iter : leaders) {
			if (bm.GetScore() > iter.first) {
				ChangeOnInitials();
				return;
			}
		}
		currentState = LEADERBORD;
		return;
	}
	w->clear();
	w->draw(activeText);
	w->display();
}

void IoManager::NewTopScore(){
	w->clear();
	w->draw(score);
	w->draw(activeText);
	w->draw(initials);
	w->display();
}

void IoManager::LeaderBoard(){
	w->clear();
	w->draw(score);
	w->draw(activeText);
	w->display();
}

void IoManager::ChangeOnStart(){
	currentState = START;
	leaders.clear();
	initials.setString("___");
	activeText.setString(S_PRESS_ANY_KEY);
}

void IoManager::ChangeOnGame(){
	currentState = IN_GAME;
	bm.StartGame();

}

void IoManager::ChangeOnOver(){
	currentState = GAME_OVER;
	activeText.setString(S_GAME_OVER);
}

void IoManager::ChangeOnInitials(){
	currentState = NEW_TOP_SCORE;
	activeText.setString(S_NEW_HIGH_SCORE);
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
	initials.setString("a__");
}

void IoManager::ChangeOnLeaderboard(){
	currentState = LEADERBORD;
	activeText.setString(S_HIGH_SCORE);
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
		ChangeOnGame();
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
		ChangeOnStart();
		return;
	}	
}

void IoManager::KeyboardReleaseCheck() {
	static std::string l_initials;
	static char activeSymbol = 65;
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
			initials.setString(l_initials);
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
			ChangeOnLeaderboard();
			UpdateLeaderbord(l_initials);
			return;
		}
		break;
	}
}

void IoManager::UpdateLeaderbord(const std::string& newInitials){
	leaders.push_back(std::make_pair(bm.GetScore(), newInitials));
	std::cout << std::to_string(bm.GetScore()) << std::endl;
	std::sort(leaders.begin(), leaders.end(), []
											(std::pair<int, std::string> f,
												std::pair<int, std::string> s)
											{return f.first > s.first; });
	if (leaders.size() > 10) leaders.pop_back();
	currentState = LEADERBORD;
	int counter = 0;
	for (auto& iter : leaders) {
		++counter;
		activeText.setString(activeText.getString() + "\n" + std::to_string(counter) + ". " + iter.second + " - " + std::to_string(iter.first));
	}
	activeText.setPosition(WIDTH / 2 - activeText.getGlobalBounds().width / 2, HEIGHT / 2 - activeText.getGlobalBounds().height / 2);
	std::ofstream fout("leaders.txt");
	fout << leaders.size() << std::endl;
	for (auto& iter : leaders) {
		fout << iter.second << " " << iter.first << std::endl;
	}
	fout.close();
}
