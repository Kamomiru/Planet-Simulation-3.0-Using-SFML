#pragma once

enum class ProgramModeID {
	NONE,
	Base,
	Startup,
	DisplayTest,
	SimulationTest,
	SimulationSetup,
	//Simulation,
	//Settings
};

//Class Interface for different program modes
//All virtual functions (that are also declared as = 0;) must be overwritten in derived classes
class BaseMode {
public:

	virtual ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) = 0;
	virtual void update() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual ~BaseMode() = default;
	ProgramModeID modeID = ProgramModeID::Base;
	
	ProgramModeID getModeID() const {
		return modeID;
	}
};

