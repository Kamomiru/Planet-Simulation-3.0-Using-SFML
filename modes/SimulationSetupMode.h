#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../src/programConfig.h"
#include "../src/programWindow.h"
#include "../src/promptWindow.h"
#include "../src/simulation.h"
#include "../src/vectorDrawer.h"
#include <any>

enum class SetupState {
	ObjectName,
	ObjectMass,
	ObjectPosition,
	ObjectRadius,
	ObjectVelocity,
	//ObjectColor
	ObjectFinalization,
	Idle
};

static class SimulationSetupMode : public BaseMode {
private:
	SetupState setupState;

	//UI Windows
	PromptWindow promptWindow;
	InputWindow inputWindow;

	//Simulation Specific Objects
	Simulation simulation;
	CelestialObject setupObj;

	//celestialObject placeholder
	sf::CircleShape setupObjShape;
	float setupObjRadius;
	bool drawSetupObj = false;

	//for SetupState::ObjectVeclocity
	bool drawVelocityVector = false;
	sf::Vector2f distanceVector;
	float vectorLength = 0;
	float vectorAngle = 0;

public:
	SimulationSetupMode() {
		modeID = ProgramModeID::SimulationSetup;
		setupState = SetupState::ObjectName;

		//UI Windows
		promptWindow = PromptWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin }, { 500.0f, 200.0f }, { "The Simulation Setup Mode is under construction!"});
		promptWindow.autoWindowSpacing();

		inputWindow = InputWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin + 100 }, { 0,0 }, { "Please enter the Planets Name:" }, 1);
		inputWindow.autoWindowSpacing();

		//celestialObject placeholder
		setupObjRadius = 10.0f;
		setupObjShape.setRadius(setupObjRadius);
		setupObjShape.setOrigin({ 10.0f , 10.0f });
		setupObjShape.setFillColor(sf::Color::Black);
		setupObjShape.setPointCount(90);

		
	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		// handle events specific to simulation setup mode

		//handle events according to setup state
		switch (setupState) {

		case(SetupState::ObjectName):
			if (eventPtr->is<sf::Event::KeyPressed>()) {

				std::optional<std::string> inputString = inputWindow.handleInputEvent(eventPtr); //Create std::optional Ptr that optionally holds a string. If it doesnt it returns a placeholder value std::nullopt
				if (inputString.has_value()) {
					std::cout << "Recieved String for Name: " << *inputString << std::endl;
					CelestialObject::Builder::setName(setupObj, *inputString);
					
					//Change inputWindow text and advance setupState to ObjectMass 
					inputWindow.setTextLineString("Please enter the Planets Mass: ", 0);
					setupState = SetupState::ObjectMass;
				}
			}
			break;
		
		case(SetupState::ObjectMass):
			if (eventPtr->is<sf::Event::KeyPressed>()) {

				std::optional<std::string> inputString = inputWindow.handleInputEvent(eventPtr, true); //Create std::optional Ptr that optionally holds a string. If it doesnt it returns a placeholder value std::nullopt
				if (inputString.has_value()) {
					std::cout << "Recieved String for Mass: " << *inputString << std::endl;
					CelestialObject::Builder::setMass(setupObj, std::stod(*inputString));

					//erase input line
					inputWindow.eraseTextLine(1);

					//Change inputWindow text and advance setupState to ObjectPosition
					inputWindow.setTextLineString("Please position the Object:", 0);
					inputWindow.autoWindowSpacing();
					setupState = SetupState::ObjectPosition;
				}
			}
			break;

		case(SetupState::ObjectPosition):
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				sf::Vector2i mouseClickPos = eventPtr->getIf<sf::Event::MouseButtonPressed>()->position;
				std::cout << "Recieved mouse Position: x = " << mouseClickPos.x << ", y = " << mouseClickPos.y << "\n";

				CelestialObject::Builder::setPosition(setupObj, { (double)mouseClickPos.x, (double)mouseClickPos.y });

				setupObjShape.setPosition({ (float)mouseClickPos.x, (float)mouseClickPos.y });

				//start drawing setupObj from now on
				drawSetupObj = true;

				//Change inputWindow text
				inputWindow.setTextLineString("Please choose your planets Radius:", 0);
				inputWindow.autoWindowSpacing();

				//advance setupState to ObjectRadius
				setupState = SetupState::ObjectRadius;
			}


			break;

		case(SetupState::ObjectRadius):
			if (eventPtr->is<sf::Event::MouseMoved>()) { //This if Statement is only here to avoid C2360 initialization of ... is skipped by 'case' label. To improove readability and decrease cluttering elsewhere. also simplifies the code
				//calculate distanceVector between mouse and current setup objects position
				sf::Vector2f mousePos = { (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.x, (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.y };
				sf::Vector2f setupObjPos = setupObjShape.getPosition();
				sf::Vector2f distanceVector = mousePos - setupObjPos;
				setupObjRadius = distanceVector.length();

				//set setupObjs radius to that distance
				setupObjShape.setRadius(setupObjRadius);
				setupObjShape.setOrigin({ setupObjRadius, setupObjRadius });

			}
			
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				//Set Radius
				CelestialObject::Builder::setRadius(setupObj, setupObjRadius);

				//advance setupState to ObjectVelocity
				setupState = SetupState::ObjectVelocity;
			}

			break;

		case(SetupState::ObjectVelocity):
			drawVelocityVector = true;
			if (eventPtr->is<sf::Event::MouseMoved>()) {
				sf::Vector2f mousePos = { (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.x, (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.y };
				sf::Vector2f setupObjPos = setupObjShape.getPosition();
				distanceVector = mousePos - setupObjPos;

				vectorLength = distanceVector.length();
				vectorAngle = distanceVector.angle().asDegrees();
			}

			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				//Set Starting Velocity
				CelestialObject::Builder::setVelocity(setupObj, { distanceVector.x , distanceVector.y });

				//advance setupState to ObjectFinalization
				setupState = SetupState::ObjectFinalization;
			}
			break;

		case(SetupState::ObjectFinalization):
			CelestialObject::Builder::finalizeSetup(setupObj);
			Simulation::Builder::addCelestialObject(simulation, setupObj);
			
			setupState = SetupState::Idle;
			break;

		case(SetupState::Idle):
			break;
		}
		
		//Returning ProgramModeID::NONE so the modeHandler does not change the Program Mode
		return ProgramModeID::NONE;
	}

	void update() override {
		// update simulation setup specific logic here
	}

	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);

		// Render simulation setup specific content here
		promptWindow.draw(&window);
		inputWindow.draw(&window);
		if (drawSetupObj) {
			window.draw(setupObjShape); 
		}
		if (drawVelocityVector) {
			drawVector(setupObjShape.getPosition(), vectorLength, vectorAngle, &window);
		}

		window.display();
	}
};

