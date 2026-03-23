#pragma once
#include "baseMode.h"
#include "../src/simulation.h"
#include "programWindow.h"
#include "../src/promptWindow.h"
#include "../helpers/stringHelper.h"

static class SimulationTestMode : public BaseMode {
private:
	inline static Simulation sim;
	inline static bool simInitialized;

	PromptWindow infoWindow;

public:

	SimulationTestMode() {
		modeID = ProgramModeID::SimulationTest;
		simInitialized = false;
		enableViewMovement = true;
		enableViewZoom = true;

		//Setup UI windows
		infoWindow = PromptWindow(conf::ui::marginVector, { 450,40 }, std::vector<std::string>{"Relative System Energy Increase: 5.00 % ", "Simulation Step:"});
		infoWindow.autoWindowSpacing();
		infoWindow.autoTextSpacing();
	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		// handle events specific to display test mode
		return ProgramModeID::NONE;
	}

	void update() override {
		sim.updateSimulationVerlet();
		//std::cout << "Simulation Step: " << sim.steps << std::endl;
		if (sim.steps > 2) {
			float relativeEnergyIncrease = sim.calcRelativeEnergyIncrease();
			std::cout << relativeEnergyIncrease << std::endl;
			std::string str = "Relative System Energy Increase:" + removeTrailingZeros(std::to_string(relativeEnergyIncrease)) + "%";
			infoWindow.setTextLineString(str, 0);
			std::string str2 = "Simulation Step: " + std::to_string(sim.steps);
			infoWindow.setTextLineString(str2, 1);
		}
		
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

		window.setView(guiView);
		infoWindow.draw(&window);

		window.display();
	}

	static void initializeSimulation(const Simulation& simulation) {
		sim = simulation;
		simInitialized = true;
	}
};