#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"
#include "../src/promptWindow.h"

static class DisplayTestMode : public BaseMode {
private:
	PromptWindow promptWindow;
public:
	DisplayTestMode() {
		modeID = ProgramModeID::DisplayTest;
		promptWindow = PromptWindow({ 100.0, 100.0f }, { 300.0, 200.0f }, std::vector<std::string>({ "SHello gg World!", "Hello g World!", "Das ist ein Prompt Window!", "Ich teste gerade die automatische Text Spacing Funktion!", "So far so good!"}));
		promptWindow.autoWindowSpacing();

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
		promptWindow.draw(&window);
		window.display();
	}
};