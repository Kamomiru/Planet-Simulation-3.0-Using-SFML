#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../src/programConfig.h"
#include "../src/programWindow.h"
#include <any>

static class SimulationSetupMode : public BaseMode {
private:
	std::any modeName = sf::Text(*conf::font::CascadiaSemiBoldPtr);
public:
	SimulationSetupMode() {
		modeID = ProgramModeID::SimulationSetup;

		std::any_cast<sf::Text&>(modeName).setFillColor(sf::Color::Black);
		std::any_cast<sf::Text&>(modeName).setPosition({ windowCenter.x, windowCenter.y - windowSize.y / 2.0f * 5.0f / 6.0f });
		std::any_cast<sf::Text&>(modeName).setCharacterSize(40);
		std::any_cast<sf::Text&>(modeName).setString("Simulation Setup Mode");
	}

	ProgramModeID handleEvent(const sf::Event& event) override {
		// handle events specific to simulation setup mode
		return ProgramModeID::NONE;
	}

	void update() override {
		// update simulation setup specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		// Render simulation setup specific content here
		window.draw(std::any_cast<sf::Text&>(modeName));
		window.display();
	}
};