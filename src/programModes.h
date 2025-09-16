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
	float rectangleMargin = 32.0f;
	float rectanglePadding = 48.0f;
	float rectangleLength = windowSize.x - 2 * rectangleMargin;
	float rectangleHeigth = windowSize.y - 2 * rectangleMargin;
	sf::Clock timer;
	
	//sf::Text title2(*conf::font::CascadiaRegularPtr); This doesn't work
	std::any title = sf::Text(*conf::font::CascadiaSemiBoldPtr);			 //Same weird Workaround as in SFML Sanbox. Is necessary due to sf::Text not having a copy constructor/default constructor.
	//TODO: Add explanation for std::any workaround

	std::any infoText = sf::Text(*conf::font::CascadiaRegularPtr);
	std::any startText = sf::Text(*conf::font::RobotoMediumPtr);
	uint16_t startTextState = 0;
	std::vector<std::string> startTextStrings;
public:

	StartupMode() { 
		modeID = ProgramModeID::Startup;
		
		roundedRect = RoundedRect(windowCenter, rectangleLength , rectangleHeigth, 8, 16, sf::Color(38, 45, 53, 255));
		roundedRect.setOriginToCenter();

		//Title Text Setup
		std::any_cast<sf::Text&>(title).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(title).setPosition({ windowCenter.x, windowCenter.y - windowSize.y / 2.0f * 5.0f/6.0f });
		std::any_cast<sf::Text&>(title).setCharacterSize(40);
		std::any_cast<sf::Text&>(title).setString("Orbit Simulation");
		//Center Title Origin
		sf::FloatRect titleBounds = std::any_cast<sf::Text&>(title).getLocalBounds();
		std::any_cast<sf::Text&>(title).setOrigin({ titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f });

		//infoText Setup
		std::any_cast<sf::Text&>(infoText).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(infoText).setPosition({ rectangleMargin + rectanglePadding, rectangleMargin + rectanglePadding + 60.0f});
		std::any_cast<sf::Text&>(infoText).setCharacterSize(20);
		std::any_cast<sf::Text&>(infoText).setString("This is a program to simulate the orbits of celestial objects. \nIt currently uses Verlet Integration to approximate the orbits.\nYou can set the starting attributes yourself or load presets.");

		//StartText Setup
		startTextStrings = std::vector<std::string>{
			"Press SPACE to continue",
			"Press SPACE to continue.",
			"Press SPACE to continue..",
			"Press SPACE to continue..." };
		std::any_cast<sf::Text&>(startText).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(startText).setPosition({windowCenter.x, windowSize.y - rectanglePadding - rectangleMargin - 60.0f});
		std::any_cast<sf::Text&>(startText).setCharacterSize(20);
		std::any_cast<sf::Text&>(startText).setString("Press SPACE to continue");
		//Center StartText Origin
		sf::FloatRect startTextBounds = std::any_cast<sf::Text&>(startText).getLocalBounds();
		std::any_cast<sf::Text&>(startText).setOrigin({ startTextBounds.size.x / 2.0f, startTextBounds.size.y / 2.0f });
	}

	ProgramModeID handleEvent(const sf::Event& event) override {
		// handle events specific to startup mode
		if (event.getIf<sf::Event::KeyPressed>() && event.getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
			std::cout << "Space!\n";
			return ProgramModeID::SimulationSetup;
		}
		
	}

	void update() override {
		if (timer.getElapsedTime().asSeconds() > 0.5f) {
			timer.restart();
			if (startTextState == 3) {
				startTextState = 0;
			}
			else {
				startTextState++;
			}
			std::any_cast<sf::Text&>(startText).setString(startTextStrings[startTextState]);
		}

	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(roundedRect);
		window.draw(std::any_cast<sf::Text&>(title));
		window.draw(std::any_cast<sf::Text&>(infoText));
		window.draw(std::any_cast<sf::Text&>(startText)); //TODO: Find out and fix why this Text element is a bit blurred
		window.display();
	}

};

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

static class SimulationTestMode : public BaseMode {
private:
	inline static Simulation sim;
	inline static bool simInitialized;
public:
	SimulationTestMode() {
		modeID = ProgramModeID::SimulationTest;
		simInitialized = false;
	}

	ProgramModeID handleEvent(const sf::Event& event) override {
		// handle events specific to display test mode
		return ProgramModeID::NONE;
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

