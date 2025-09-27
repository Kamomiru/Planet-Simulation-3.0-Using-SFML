#pragma once
#include "programWindow.h"

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

	//Setting up Variables for View Movement
	bool enableViewMovement = false;
	bool enableViewZoom = false;
	bool mouseDragged = false;
	sf::Vector2f lastMousePos = { 0.0f, 0.0f };
	sf::Vector2f currentMousePos;



	void updateView(std::optional<sf::Event> eventPtr) {
		if (this->enableViewMovement) {
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				mouseDragged = true;
				lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), guiView);
			}
			else if (eventPtr->is<sf::Event::MouseButtonReleased>()) {
				mouseDragged = false;
			}
			if (mouseDragged) {
				currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), guiView); //Gets current mouse position in a given window and currentView. Then converts the PixelsCoordinates to usable coordinates

				sf::Vector2f mouseDelta = lastMousePos - currentMousePos;
				currentView.move(mouseDelta); //moove currentView by our mouseDelta

				lastMousePos = currentMousePos;
			}
		}

		if (this->enableViewZoom) {
			if (const sf::Event::MouseWheelScrolled* mouseWheelScrolled = eventPtr->getIf<sf::Event::MouseWheelScrolled>()) { //Here we DO need the event later in our program. so we have to get it using the getIf<T>() function
				if (mouseWheelScrolled->delta > 0) { //here "mouseWheelScrolled" is an event pointer so it could also be dereferenced by (*mouseWheelScrolled).delta
					currentView.zoom(0.9f);
				}
				if (mouseWheelScrolled->delta < 0) {
					currentView.zoom(1.1f);
				}

			}
		}
	}
};

