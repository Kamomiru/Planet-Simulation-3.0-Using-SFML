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

	void setMode(ProgramModeID newMode) {
		if (newMode != ProgramModeID::NONE && newMode != currentMode->getModeID()) { //Only change mode if current newMode is not currentMode and not NONE
			switch (newMode) {
			case ProgramModeID::Startup:
				setMode(std::make_unique<StartupMode>());
				break;
			case ProgramModeID::DisplayTest:
				setMode(std::make_unique<DisplayTestMode>());
				break;
			case ProgramModeID::SimulationTest:
				setMode(std::make_unique<SimulationTestMode>());
				break;
			case ProgramModeID::SimulationSetup:
				setMode(std::make_unique<SimulationSetupMode>());
				break;
			default:
				break;
			}
		}
	}

	//Handle events accoting to currentMode
	void handleEvent(const sf::Event& event) {
		ProgramModeID changeMode;
		changeMode = currentMode->handleEvent(event);
		
		this->setMode(changeMode); //Change mode if necessary
	}

	//Update according to currentMode
	void update() {
		currentMode->update();
	}

	//Render according to currentMode
	void render(sf::RenderWindow& window) {
		currentMode->render(window);
	}

	//Getter for currentMode's ID
	ProgramModeID getCurrentModeID() const {
		return currentMode->getModeID();
	}
};