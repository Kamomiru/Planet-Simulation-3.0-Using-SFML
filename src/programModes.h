#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>



class StartupMode : public BaseMode {
private:
	sf::RectangleShape rectangle;

public:

	StartupMode() { 
		rectangle.setSize(sf::Vector2f(200.f, 100.f));
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setPosition({ 800.0f, 600.0f }); // Center of a 1920x1080 view
	}

	void handleEvent(const sf::Event& event) override {
		// handle events specific to menu mode
	}
	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(rectangle);
		window.display();
	}

};

class DisplayTestMode : public BaseMode {
private:
	sf::CircleShape circle;
public:
	DisplayTestMode() {
		circle.setRadius(50.f);
		circle.setFillColor(sf::Color::Black);
		circle.setPosition({ 600.0f, 400.0f });
	}
	void handleEvent(const sf::Event& event) override {
		// handle events specific to display test mode
	}
	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(circle);
		window.display();
	}
}; 