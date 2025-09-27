#pragma once
#include "StartupMode.h"
#include "DisplayTestMode.h"
#include "SimulationTestMode.h"
#include "SimulationSetupMode.h"
#include <optional>

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
	//We can also add code here that every mode has to handle. for example Window Movement
	void handleEvent(const std::optional<sf::Event> eventPtr) {

		currentMode->updateView(eventPtr); // if you wish to enable this you have to set enableViewMovement / enableViewZoom to true in that specific Mode! otherwise this fuction does nothing!
		

		ProgramModeID changeMode = currentMode->handleEvent(eventPtr); //Is of type ProgramModeID so the Modes can return another mode ID they would like to change to.
		
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