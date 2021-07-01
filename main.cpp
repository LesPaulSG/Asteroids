#include <fstream>

#include "IO_Thread.h"

int main(){
	BulletManager bulletManager;

	//bulletManager.CreateWall(sf::Vector2f(5, 0), sf::Vector2f(5, HEIGHT));					//default border undestructable walls
	//bulletManager.CreateWall(sf::Vector2f(WIDTH, 0), sf::Vector2f(WIDTH, HEIGHT));			//
	//bulletManager.CreateWall(sf::Vector2f(0, 5), sf::Vector2f(WIDTH, 5));					//
	//bulletManager.CreateWall(sf::Vector2f(0, HEIGHT), sf::Vector2f(WIDTH, HEIGHT));			//

	/*std::ifstream fin;
	fin.open("Walls.txt");
	int quantity;
	fin >> quantity;
	for (int i = 0; i < quantity; ++i) {
		int A, B, C, D;
		fin >> A >> B >> C >> D;
		bulletManager.CreateWall(sf::Vector2f(A, B), sf::Vector2f(C, D));
	}
	fin.close();*/
	
	std::chrono::duration<float> time{ 0 };
	bool gameOver = false;

	std::thread IOThread(input, std::ref(bulletManager), std::ref(time), std::ref(gameOver));		//input output thread
	IOThread.detach();

	//std::this_thread::sleep_for(std::chrono::seconds(3));
	auto clock = std::chrono::high_resolution_clock::now();											//upper

	while (!gameOver){
		bulletManager.Update(time.count());

		time  = std::chrono::high_resolution_clock::now() - clock;
		clock = std::chrono::high_resolution_clock::now();
	}

	return 0;
}