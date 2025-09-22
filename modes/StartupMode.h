#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../sfml-entities/customSFMLEntities.h"
#include "../src/programConfig.h"
#include "../src/programWindow.h"
#include <any>

static class StartupMode : public BaseMode {
private:
	RoundedRect roundedRect;
	float rectangleMargin = 32.0f;
	float rectanglePadding = conf::ui::generalSidePadding;
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

		roundedRect = RoundedRect(windowCenter, rectangleLength, rectangleHeigth, 16, conf::ui::windowBackgroundColor);
		roundedRect.setOriginToCenter();

		//Title Text Setup
		std::any_cast<sf::Text&>(title).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(title).setPosition({ windowCenter.x, windowCenter.y - windowSize.y / 2.0f * 5.0f / 6.0f });
		std::any_cast<sf::Text&>(title).setCharacterSize(40);
		std::any_cast<sf::Text&>(title).setString("Orbit Simulation");
		//Center Title Origin
		sf::FloatRect titleBounds = std::any_cast<sf::Text&>(title).getLocalBounds();
		std::any_cast<sf::Text&>(title).setOrigin({ titleBounds.size.x / 2.0f, titleBounds.size.y / 2.0f });

		//infoText Setup
		std::any_cast<sf::Text&>(infoText).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(infoText).setPosition({ rectangleMargin + rectanglePadding, rectangleMargin + rectanglePadding + 60.0f });
		std::any_cast<sf::Text&>(infoText).setCharacterSize(20);
		std::any_cast<sf::Text&>(infoText).setString("This is a program to simulate the orbits of celestial objects. \nIt currently uses Verlet Integration to approximate the orbits.\nYou can set the starting attributes yourself or load presets.");

		//StartText Setup
		startTextStrings = std::vector<std::string>{
			"Press SPACE to continue",
			"Press SPACE to continue.",
			"Press SPACE to continue..",
			"Press SPACE to continue..." };
		std::any_cast<sf::Text&>(startText).setFillColor(sf::Color::White);
		std::any_cast<sf::Text&>(startText).setPosition({ windowCenter.x, windowSize.y - rectanglePadding - rectangleMargin - 60.0f });
		std::any_cast<sf::Text&>(startText).setCharacterSize(20);
		std::any_cast<sf::Text&>(startText).setString("Press SPACE to continue");
		//Center StartText Origin
		sf::FloatRect startTextBounds = std::any_cast<sf::Text&>(startText).getLocalBounds();
		std::any_cast<sf::Text&>(startText).setOrigin({ startTextBounds.size.x / 2.0f, startTextBounds.size.y / 2.0f });
	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		// handle events specific to startup mode
		if (eventPtr->getIf<sf::Event::KeyPressed>() && eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space) {
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