#pragma once
#include "programModes.h"

class ModeHandler {
private:
	std::unique_ptr<BaseMode> currentMode;

public:
	ModeHandler(std::unique_ptr<BaseMode> startMode)
		//Initializer List - All objects here are initialized right away
		: currentMode(std::move(startMode)) { //essentialy: currentMode = startMode but using operator= is disallowed to copy unique_ptr, so we use std::move instead
		//Constructor Body
	}

	//Setter for currentMode
	void setMode(std::unique_ptr<BaseMode> newMode) {
		currentMode = std::move(newMode); //same as above
	}

	//Handle events accoting to currentMode
	void handleEvent(const sf::Event& event) {
		currentMode->handleEvent(event);
	}

	//Render according to currentMode
	void render(sf::RenderWindow& window) {
		currentMode->render(window);
	}
};