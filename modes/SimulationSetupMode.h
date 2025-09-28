#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../src/programConfig.h"
#include "../src/programWindow.h"
#include "../src/promptWindow.h"
#include "../src/simulation.h"
#include "../src/vectorDrawer.h"
#include <any>
#include <sstream> // for std::ostringstream
#include <iomanip> // for std::fixed and std::setprecision

enum class SetupState {
	ObjectName,
	ObjectMass,
	ObjectPosition,
	ObjectRadius,
	ObjectVelocity,
	ObjectColor,
	ObjectFinalization,
	ContinueRequest,
	Idle
};

static class SimulationSetupMode : public BaseMode {
private:
	SetupState setupState;

	//UI Windows
	InputWindow inputWindow;
	PromptWindow errorWindow;
	ObjectTableWindow tableWindow;

	//Simulation Specific Objects
	Simulation simulation;
	CelestialObject setupObj;

	//celestialObject placeholder
	sf::CircleShape setupObjShape;
	float setupObjRadius;
	bool drawSetupObj = false;

	//for SetupState::ObjectName
	bool drawErrorWindow = false;

	//for SetupState::ObjectRadius
	bool drawDensityIndicator = false;
	std::optional<sf::Text> densityIndicator;

	//for SetupState::ObjectVeclocity
	bool drawVelocityVector = false;
	sf::Vector2f distanceVector;
	float vectorLength = 0;
	float vectorAngle = 0;


public:
	SimulationSetupMode() {
		modeID = ProgramModeID::SimulationSetup;
		setupState = SetupState::ObjectName;

		enableViewMovement = true;

		//UI Windows
		inputWindow = InputWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin }, { 0,0 }, { "Please enter the Planets Name:" }, 1);
		inputWindow.autoWindowSpacing();
		errorWindow = PromptWindow({ conf::ui::generalSideMargin + 420, conf::ui::generalSideMargin }, { 0,0 }, { "Please enter another Name!", "<- That name was already taken..."});
		errorWindow.autoWindowSpacing();

		tableWindow = ObjectTableWindow({ conf::ui::generalSideMargin, conf::ui::generalSideMargin + 100 }, { 440.0, 800.0f });
		tableWindow.autoSetHeigth();

		//celestialObject placeholder
		setupObjRadius = 10.0f;
		setupObjShape.setRadius(setupObjRadius);
		setupObjShape.setOrigin({ 10.0f , 10.0f });
		setupObjShape.setFillColor(sf::Color::Black);
		setupObjShape.setPointCount(60);

		//for SetupState::ObjectRadius
		densityIndicator = sf::Text(*conf::font::CascadiaRegularPtr);
		densityIndicator->setCharacterSize(18);
		densityIndicator->setFillColor(sf::Color::Black);
		
	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		

		//handle events according to setup state
		switch (setupState) {

		case(SetupState::ObjectName):
			if (eventPtr->is<sf::Event::KeyPressed>()) {

				//InputWindow::handleInputEvent also checks wether input is confirmed via the enter key!
				std::optional<std::string> inputString = inputWindow.handleInputEvent(eventPtr); //Create std::optional Ptr that optionally holds a string. If it doesnt it returns a placeholder value std::nullopt
				if (inputString.has_value()) {

					//Check if Name is already used in simulation!
					if (isElementOfVector(*inputString, simulation.getObjectNames())) {
						drawErrorWindow = true;
						inputWindow.resetInput();
					}
					else {
						std::cout << "Recieved String for Name: " << *inputString << std::endl;
						CelestialObject::Builder::setName(setupObj, *inputString);

						drawErrorWindow = false;

						//Change inputWindow text and advance setupState to ObjectMass 
						inputWindow.setTextLineString("Please enter the Planets Mass: ", 0);
						setupState = SetupState::ObjectMass;
					}
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
					//inputWindow.eraseTextLine(1);

					//Change inputWindow text and advance setupState to ObjectPosition
					inputWindow.setTextLineString("Please position the Object:", 0);
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
				inputWindow.setTextLineString("Please choose your objects", 0);
				inputWindow.setTextLineString("Radius:", 1);

				//advance setupState to ObjectRadius
				setupState = SetupState::ObjectRadius;
				//start drawing densityIndicator
				drawDensityIndicator = true;
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

				//Handle density indicator
				float pi = 3.14159265359;
				float density = setupObj.getMass() / (4.0 / 3.0 * pi * std::pow(setupObjRadius / constants::scale, 3.0f)) / 1000.0f;
				std::cout << density << "\n";
				std::ostringstream oss;
				oss << std::setprecision(2) << std::fixed << density << " g/cm³";

				densityIndicator->setString(oss.str());
				densityIndicator->setPosition(mousePos + sf::Vector2f(distanceVector.x * 0.5, distanceVector.y * 0.5));
			}
			
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				//Set Radius
				CelestialObject::Builder::setRadius(setupObj, setupObjRadius);

				//advance setupState to ObjectVelocity
				setupState = SetupState::ObjectVelocity;
				//stop drawing densityIndicator
				drawDensityIndicator = false;

				//Change inputWindow text
				inputWindow.setTextLineString("Please choose your objects", 0);
				inputWindow.setTextLineString("starting velocity:", 1);

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
				setupState = SetupState::ObjectColor;
			}
			break;

		case(SetupState::ObjectColor):
			//TODO: plan how to implement this and implement
			CelestialObject::Builder::setColor(setupObj, sf::Color::Black);
			setupState = SetupState::ObjectFinalization;

			break;

		case(SetupState::ObjectFinalization):
			//Finalize setup and add CelestialObject to our simulation
			CelestialObject::Builder::finalizeSetup(setupObj);
			Simulation::Builder::addCelestialObject(simulation, setupObj);
			
			tableWindow.appendLine(setupObj.getName(), setupObj.getMass(), setupObj.getRadius());

			//reset to beginning
			drawSetupObj = false;
			drawVelocityVector = false;

			setupObj = CelestialObject();

			inputWindow.setTextLineString("Would you like to add", 0);
			inputWindow.setTextLineString("another object? [Y/N]", 1);

			setupState = SetupState::ContinueRequest;
			
			
				
			
			break;

		case(SetupState::ContinueRequest):

			if (eventPtr->is<sf::Event::KeyPressed>()) {
				if (eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Y) {
					inputWindow.setTextLineString("Please enter the Planets Name:", 0);
					inputWindow.setTextLineString("", 1);
					setupState = SetupState::ObjectName;
				}
				
				if (eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::N) {
					//TODO: Eventually implement exception for only one initialized CelestialObject
					Simulation::Builder::finalizeSetup(simulation);
					SimulationTestMode::initializeSimulation(simulation);
					return ProgramModeID::SimulationTest;
				}
			}
			

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

		//Draw UI Windows
		//window.setView(guiView);
		inputWindow.draw(&window);
		tableWindow.draw(&window);
		if (drawErrorWindow) {
			errorWindow.draw(&window);
		}

		//Draw other Objects
		/*window.setView(currentView);*/
		if (drawDensityIndicator) {
			window.draw(*densityIndicator);
		}

		//draw all CelestialObjects that have been finished the setup process
		for (sf::CircleShape obj : simulation.getObjectDrawables()) {
			window.draw(obj);
		}
		//draw all CelestialObjects starting Velocity vector
		for (CelestialObject obj : simulation.celestialObjectContainer) {
			sf::Vector2f startingVelocityVector = obj.getStartingVelocity();
			drawVector(obj.getPositionSf(), startingVelocityVector.length(), startingVelocityVector.angle().asDegrees(), &window);
		}

		//Drawing setupObj
		if (drawSetupObj) {
			window.draw(setupObjShape); 
		}
		if (drawVelocityVector) {
			drawVector(setupObjShape.getPosition(), vectorLength, vectorAngle, &window);
		}

		window.display();
	}
};

