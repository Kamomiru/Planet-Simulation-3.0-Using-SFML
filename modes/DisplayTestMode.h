#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>

static class DisplayTestMode : public BaseMode {
private:
	sf::CircleShape circle;
public:
	DisplayTestMode() {
		modeID = ProgramModeID::DisplayTest;
		circle.setRadius(50.f);
		circle.setFillColor(sf::Color::Black);
		circle.setPosition({ 600.0f, 400.0f });
	}

	ProgramModeID handleEvent(const sf::Event& event) override {
		// handle events specific to display test mode
		return ProgramModeID::NONE;
	}

	void update() override {
		// update display test specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(circle);
		window.display();
	}
};