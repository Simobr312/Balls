#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>

#include "Ball.h"

void PollEvents(sf::RenderWindow& window);
sf::Vector2f mousePosition;

int main() {

	sf::RenderWindow window;
	window.create({ 1280, 720 }, "Balls", sf::Style::Close);

	std::vector<Ball> balls;

	srand((unsigned)time(0));

	for (int i = 0; i < 50; ++i)
		balls.push_back(Ball((i+1), sf::Vector2f(rand() % window.getSize().x, rand() % window.getSize().y), float(rand() % 50) + 25));

	while (window.isOpen()) {

		PollEvents(window);

		window.clear();

		for (Ball& ball : balls) {
			ball.Uptade(window);
			ball.MouseCollider(mousePosition);
			ball.ScreenCollider(window);
			ball.InputPush(mousePosition, window);
			for (Ball& other : balls) {
				if(ball.GetID() != other.GetID())
					ball.CheckCollision(other, ball.GetRadius()/other.GetRadius());
			}
		}


		for (Ball& ball : balls)
			ball.Draw(window);


		window.display();

	}
	return 0;
}

void PollEvents(sf::RenderWindow& window) {
	sf::Event e;
	while (window.pollEvent(e)) {
		if (e.type == sf::Event::Closed)
		{
			window.close();
		}
		if (e.type == sf::Event::Resized) {
			//Camera settings
		}
		if (e.type == sf::Event::MouseMoved) {
			mousePosition.x = e.mouseMove.x;
			mousePosition.y = e.mouseMove.y;
		}
	}
}
