#pragma once
#include "baseMode.h"
#include "../src/simulation.h"
#include "programWindow.h"

static class SimulationTestMode : public BaseMode {
private:
	inline static Simulation sim;
	inline static bool simInitialized;

public:

	SimulationTestMode() {
		modeID = ProgramModeID::SimulationTest;
		simInitialized = false;
		enableViewMovement = true;
		enableViewZoom = true;
	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		// handle events specific to display test mode
		return ProgramModeID::NONE;
	}

	void update() override {
		sim.updateSimulationVerlet();
		std::cout << "Simulation Step: " << sim.steps << std::endl;
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.setView(currentView);
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