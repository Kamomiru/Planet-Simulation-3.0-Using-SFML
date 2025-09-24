#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"
#include "../src/promptWindow.h"

static class DisplayTestMode : public BaseMode {
private:
	InputWindow inputWindow;
public:
	DisplayTestMode() {
		modeID = ProgramModeID::DisplayTest;
		inputWindow = InputWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin }, { 300.0, 200.0f }, std::vector<std::string>({"Das ist ein Input Window!", "Bitte gib mir deinen Input:", "Über mir sollte jetzt dein Input zu sehen sein..."}), 2);
		inputWindow.autoWindowSpacing();

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
		inputWindow.draw(&window);
		window.display();
	}
};