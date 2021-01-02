#pragma once
#include <SFML/Graphics.hpp>

class Ball
{
private:
	int ID;
	sf::CircleShape body;
	float mass;

	sf::Vector2f velocity;

public:
	Ball(const int ID, sf::Vector2f position, float radius);
	~Ball();

	void Uptade(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);

	void Move(float dx, float dy) { body.move(dx, dy); }

	void MouseCollider(sf::Vector2f mousePos);
	bool ScreenCollider(sf::RenderWindow& window);
	bool CheckCollision(Ball& other, float push);
	void InputPush(sf::Vector2f mousePos, sf::RenderWindow& window);

	int GetID() { return ID; }
	float GetRadius() { return body.getRadius(); }
	float GetMass() { return mass; }
	sf::Vector2f GetPosition() { return body.getPosition(); }
	sf::Vector2f GetVelocity() { return velocity; }

	void SetVelocityX(float xv) { velocity.x = xv; }
	void SetVelocityY(float yv) { velocity.y = yv; }

public:
	static int IDselected;
	static int IDpushing;
};