#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"
#include "../src/promptWindow.h"

static class DisplayTestMode : public BaseMode {
private:
	InputWindow inputWindow;
	ObjectTableWindow tableWindow;
	sf::CircleShape shape;
public:
	DisplayTestMode() {
		modeID = ProgramModeID::DisplayTest;
		inputWindow = InputWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin }, { 300.0, 200.0f }, std::vector<std::string>({"Das ist ein Input Window!", "Bitte gib mir deinen Input:", "Über mir sollte jetzt dein Input zu sehen sein..."}), 2);
		inputWindow.autoWindowSpacing();

		tableWindow = ObjectTableWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin + 200 }, { 440.0, 800.0f });
		tableWindow.appendLine("Erde", 100000, 10);
		tableWindow.appendLine("Mond", 100000, 10);


		shape = sf::CircleShape(100, 60);
		shape.setFillColor(sf::Color::Black);
		shape.setPosition({ 600, 600 });

		enableViewMovement = true;
		enableViewZoom = true;

	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		// handle events specific to display test mode
		if (eventPtr->is<sf::Event::KeyPressed>()) {
			inputWindow.handleInputEvent(eventPtr, true);
		}
		return ProgramModeID::NONE;
	}

	void update() override {
		// update display test specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);

		window.setView(guiView);
		inputWindow.draw(&window);
		tableWindow.draw(&window);
		window.setView(currentView);
		window.draw(shape);
		window.display();
	}
};