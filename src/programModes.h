#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "simulation.h"
#include "programConfig.h"
//#include "../helpers/customVertexArrays.h"
#include "../sfml-entities/customSFMLEntities.h"
#include <any>

//inline sf::Font font("../resources/CascadiaCode-Regular.ttf");

static class StartupMode : public BaseMode {
private:
	RoundedRect roundedRect;
	float rectangleLength = 1000.0f;
	//sf::Text title2(*conf::font::CascadiaRegularPtr); This doesn't work
	std::any title;			 //Same weird Workaround as in SFML Sanbox. Is necessary due to sf::Text not having a copy constructor/default constructor.
	//TODO: Add explanation for std::any workaround
public:

	StartupMode() { 
		modeID = ProgramModeID::Startup;

		roundedRect = RoundedRect(windowCenter, rectangleLength * 1.618f, rectangleLength, 8, 16, sf::Color(38, 45, 53, 255));
		roundedRect.setOriginToCenter();

		title = sf::Text(*conf::font::CascadiaRegularPtr);
		std::any_cast<sf::Text&>(title).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(title).setPosition(windowCenter);
		std::any_cast<sf::Text&>(title).setCharacterSize(40);
		std::any_cast<sf::Text&>(title).setString("Orbit Simulation");

		sf::FloatRect titleBounds = std::any_cast<sf::Text&>(title).getLocalBounds();

		std::any_cast<sf::Text&>(title).setOrigin({ titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f });

		std::cout << "Title Bounds: " << titleBounds.size.x << ", " << titleBounds.size.y << std::endl;

	}

	void handleEvent(const sf::Event& event) override {
		// handle events specific to menu mode
	}

	void update() override {
		// update menu specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(roundedRect);
		window.draw(std::any_cast<sf::Text>(title));
		window.display();
	}

};

static class SimulationSetupMode : public BaseMode {
private:

public:
	SimulationSetupMode() {
		modeID = ProgramModeID::SimulationSetup;
	}

	void handleEvent(const sf::Event& event) override {
		// handle events specific to simulation setup mode
	}

	void update() override {
		// update simulation setup specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		// Render simulation setup specific content here
		window.display();
	}
};

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

	void handleEvent(const sf::Event& event) override {
		// handle events specific to display test mode
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

static class SimulationTestMode : public BaseMode {
private:
	inline static Simulation sim;
	inline static bool simInitialized;
public:
	SimulationTestMode() {
		modeID = ProgramModeID::SimulationTest;
		simInitialized = false;
	}

	void handleEvent(const sf::Event& event) override {
		// handle events specific to display test mode
	}

	void update() override {
		sim.updateSimulationVerlet();
		std::cout << "Simulation Step: " << sim.steps << std::endl;
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::Black);
		std::vector<sf::CircleShape> drawableObjects = sim.getObjectDrawables();
		std::vector<sf::VertexArray> drawableTrajectories = sim.getTrajectoryDrawables();
		for (const sf::Drawable& drawable : drawableObjects) {
			window.draw(drawable);
		}
		for (const sf::Drawable& drawable : drawableTrajectories) {
			window.draw(drawable);
		}
		window.display();
	}

	static void initializeSimulation(const Simulation& simulation) {
		sim = simulation;
		simInitialized = true;
	}
};

