#pragma once

enum class ProgramModeID {
	Base,
	Startup,
	DisplayTest,
	//Simulation,
	//Settings
};

class BaseMode {
public:

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual ~BaseMode() = default;
	ProgramModeID modeID = ProgramModeID::Base;
	
	ProgramModeID getModeID() const {
		return modeID;
	}
};

