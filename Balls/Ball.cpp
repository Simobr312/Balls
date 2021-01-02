#include "Ball.h"
#include <iostream>

int Ball::IDselected = 0;
int Ball::IDpushing = 0;

Ball::Ball(const int ID, sf::Vector2f position, float radius)
{
	this->ID = ID;
	IDselected = 0;
	body.setPosition(position);
	body.setRadius(radius);
	body.setOrigin(radius, radius);
	body.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));

	this->mass = body.getRadius();
	
}

Ball::~Ball()
{
	
}

void Ball::Uptade(sf::RenderWindow& window)
{
	body.setPosition(body.getPosition().x + velocity.x, body.getPosition().y + velocity.y);

	velocity.y *= 0.9995;
	velocity.x *= 0.9995;
}

void Ball::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}


bool Ball::CheckCollision(Ball& other, float push) {

	sf::Vector2f Delta = { this->body.getPosition().x - other.body.getPosition().x, body.getPosition().y - other.body.getPosition().y };
	sf::Vector2f Intersect = { abs(Delta.x) - (this->body.getRadius() + other.body.getRadius() ), abs(Delta.y) - (this->body.getRadius() + other.body.getRadius()) };

	if (Intersect.x < 0.f && Intersect.y < 0.f) {

		push = std::max(0.f, std::min(push , 1.f));

		sf::Vector2f ThisInitialVel = { GetVelocity().x, GetVelocity().y };
		sf::Vector2f OtherInitialVel = { other.GetVelocity().x, other.GetVelocity().y };

		if (Intersect.x > Intersect.y) { //Right side
			if (Delta.x > 0.0f) {
				Move(-Intersect.x * (1.0f - push), 0.0f);
				other.Move(Intersect.x * push, 0.0f);
			}
			else { //Left
				Move(Intersect.x * (1.0f - push), 0.0f);
				other.Move(-Intersect.x * push, 0.0f);

			}
				SetVelocityX((ThisInitialVel.x * (GetMass() - other.GetMass()) + 2 * other.GetMass() * OtherInitialVel.x) / (GetMass() + other.GetMass()));			
		}
		else {
			if (Delta.y > 0.0f) { //Up
				Move(0.0f, -Intersect.y * (1.0f - push));
				other.Move(0.0f, Intersect.y * push);
			}
			else { //Down
				Move(0.0f, Intersect.y * (1.0f - push));
				other.Move(0.0f, -Intersect.y * push);
			}
			SetVelocityY((ThisInitialVel.y * (GetMass() - other.GetMass()) + 2 * other.GetMass() * OtherInitialVel.y) / (GetMass() + other.GetMass()));
		}

		other.SetVelocityX((OtherInitialVel.x * (other.GetMass() - GetMass()) + 2 * GetMass() * ThisInitialVel.x) / (GetMass() + other.GetMass()));
		other.SetVelocityY((OtherInitialVel.y * (other.GetMass() - GetMass()) + 2 * GetMass() * ThisInitialVel.y) / (GetMass() + other.GetMass()));

		return true;
	}

	return false;

}

void Ball::MouseCollider(sf::Vector2f mousePos)
{
	sf::Vector2f Delta = { body.getPosition().x - mousePos.x , body.getPosition().y - mousePos.y};
	sf::Vector2f Intersect = { abs(Delta.x) - body.getRadius() , abs(Delta.y) - body.getRadius() };

	if (Intersect.x < 0.f && Intersect.y < 0.f) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (IDselected == ID || IDselected == 0)) {
			this->body.setPosition(mousePos);
			IDselected = ID;
		}
		else if (IDselected == ID) IDselected = 0;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (IDpushing == 0 || IDpushing == ID)) {
			IDpushing = ID;
		}

	} 
}

bool Ball::ScreenCollider(sf::RenderWindow& window) {
	
	if (body.getPosition().x - body.getRadius() < 0.f || body.getPosition().x + body.getRadius() > window.getSize().x)
	{
		if (body.getPosition().x - body.getRadius() < 0.f) {
			Move(-(body.getPosition().x - body.getRadius()), 0.0f);
		}
		if (body.getPosition().x + body.getRadius() > window.getSize().x) {
			Move(window.getSize().x - body.getPosition().x - body.getRadius(), 0.0f);

		}
		SetVelocityX(0.f - GetVelocity().x);
		return true;
	}
	else if(body.getPosition().y - body.getRadius() < 0.0f || body.getPosition().y + body.getRadius() > window.getSize().y) 
	{
		if (body.getPosition().y - body.getRadius() < 0.0f) {
			Move(0.0f, -(body.getPosition().y - body.getRadius()));	
		}
		if (body.getPosition().y + body.getRadius() > window.getSize().y) {
			Move(0.0f, window.getSize().y - body.getPosition().y - body.getRadius());

		}	
		SetVelocityY(0.f - GetVelocity().y);
		return true;
	}
	return false;
}

const float LMAX = 200.f;

void Ball::InputPush(sf::Vector2f mousePos, sf::RenderWindow& window) {

	if (IDpushing == ID) {
		sf::Vector2f Delta = { body.getPosition().x - mousePos.x , body.getPosition().y - mousePos.y };

		if (Delta.x * Delta.x + Delta.y * Delta.y > LMAX * LMAX ) {
			float angle;

			if (mousePos.x > body.getPosition().x)
				angle = std::acos(-1) + std::atan(Delta.y / Delta.x);
			else 
				angle = std::atan(Delta.y / Delta.x);
			
			Delta.x = std::cos(angle) * LMAX;
			Delta.y = std::sin(angle) * LMAX;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vertex line[2];

			line[0] = body.getPosition();
			line[1] = sf::Vector2f((body.getPosition().x - Delta.x), (body.getPosition().y - Delta.y));

			window.draw(line, 2, sf::Lines);
		}
		else {
			velocity.x = Delta.x / 200.f;
			velocity.y = Delta.y / 200.f;
			IDpushing = 0;
		}
	}
}