#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "simulation.h"
#include "programConfig.h"
#include "../helpers/customVertexArrays.h"




static class StartupMode : public BaseMode {
private:
	sf::VertexArray backgroundRect;
	sf::RectangleShape rectangle;
	float rectangleHeight = 500.f;
public:

	StartupMode() { 
		modeID = ProgramModeID::Startup;
		backgroundRect = createRoundedRectangleBorder({ 0.0f,0.0f }, 10.0f, rectangleHeight * 1.618, rectangleHeight, 10.0f, sf::Color(50, 50, 50), 8);
		//rectangle.setSize(sf::Vector2f(rectangleHeight * 1.618, rectangleHeight));
		//rectangle.setOrigin(sf::Vector2f(rectangle.getSize().x / 2.0f, rectangle.getSize().y / 2.0f)); // Center the origin
		//rectangle.setFillColor(sf::Color::Black);
		//rectangle.setPosition(windowCenter);
	}

	void handleEvent(const sf::Event& event) override {
		// handle events specific to menu mode
	}

	void update() override {
		// update menu specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(backgroundRect);
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

