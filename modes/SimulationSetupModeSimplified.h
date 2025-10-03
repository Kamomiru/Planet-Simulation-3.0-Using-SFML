#pragma once
#include "baseMode.h"
#include <SFML/Graphics.hpp>
#include "../src/programConfig.h"
#include "../src/programWindow.h"
#include "../src/promptWindow.h"
#include "../src/simulation.h"
#include "../src/vectorDrawer.h"
#include "../src/PlanetEnum.h"
#include <any>
#include <sstream> // for std::ostringstream
#include <iomanip> // for std::fixed and std::setprecision



enum class SetupState {
	SelectObject,
	SetVelocity,
	SetName,
	AdditionalObjectRequest,
	finalize,
	Idle
};

static class SimulationSetupModeSimplified : public BaseMode {
private:
	SetupState setupState;

	std::vector<CelestialObject> planetContainer{
		CelestialObject("Earth", constants::earth::mass, constants::earth::radius, { 0,0 }, { 0,0 }, sf::Color(0, 102, 204)),   // Blue-Green
		CelestialObject("Moon", constants::moon::mass, constants::moon::radius, { 0,0 }, { 0,0 }, sf::Color(200, 200, 200)), // Gray
		CelestialObject("Mars", constants::mars::mass, constants::mars::radius, { 0,0 }, { 0,0 }, sf::Color(188, 39, 50)),   // Red
		CelestialObject("Jupiter", constants::jupiter::mass, constants::jupiter::radius, { 0,0 }, { 0,0 }, sf::Color(210, 180, 140)), // Brownish-Tan
		CelestialObject("Saturn", constants::saturn::mass, constants::saturn::radius, { 0,0 }, { 0,0 }, sf::Color(220, 206, 160)), // Pale Yellow
		CelestialObject("Uranus", constants::uranus::mass, constants::uranus::radius, { 0,0 }, { 0,0 }, sf::Color(173, 216, 230)), // Light Blue
		CelestialObject("Mercury", constants::mercury::mass, constants::mercury::radius, { 0,0 }, { 0,0 }, sf::Color(169, 169, 169)), // Dark Gray
		CelestialObject("Pluto", constants::pluto::mass, constants::pluto::radius, { 0,0 }, { 0,0 }, sf::Color(222, 184, 135)), // Pale Brown
		CelestialObject("Venus", constants::venus::mass, constants::venus::radius, { 0,0 }, { 0,0 }, sf::Color(218, 165, 32)),  // Golden-Yellow
		CelestialObject("Neptune", constants::neptune::mass, constants::neptune::radius, { 0,0 }, { 0,0 }, sf::Color(0, 0, 139))     // Deep Blue
	};
	

	CelestialObject chosenPlanet = planetContainer[Earth];
	PlanetEnum chosenPlanetEnum = Earth;

	Simulation simulation;

	//UI Windows
	PromptWindow instructionWindow;
	ObjectTableWindow tableWindow;
	InputWindow inputWindow;
	
	int objectNo = 0;

	//for SetupState setVelocity
	bool drawVelocityVector = false;
	bool drawVelocityText = false;

	std::any velocityText;

	sf::Vector2f sfDistanceVector{ 0.0f, 0.0f };
	float vectorLength = 0;
	float vectorAngle = 0;

	//for SetupState setName
	bool drawInputWindow = false;

public:
	SimulationSetupModeSimplified() {
		modeID = ProgramModeID::SimulationSetup;
		setupState = SetupState::SelectObject;

		enableViewMovement = false;

		//Setup UI windows
		instructionWindow = PromptWindow(conf::ui::marginVector, { 450,40 }, std::vector<std::string>{"Please select a Planet using the arrow keys!"});
		instructionWindow.autoTextSpacing();

		tableWindow = ObjectTableWindow(conf::ui::marginVector + sf::Vector2f{ 0.0f, 45.0f }, { 450,80 });

		tableWindow.addLine();
		tableWindow.setLineStrings(Earth, 1);

		inputWindow = InputWindow(conf::ui::marginVector + sf::Vector2f{ 0,125 }, { 450,60 }, std::vector<std::string>{"The name must be unique!"}, 1);
		inputWindow.autoTextSpacing();

		//Setup TextObj for SetupState::setVelocity
		velocityText = sf::Text(*conf::font::CascadiaRegularPtr);
		std::any_cast<sf::Text&>(velocityText).setFillColor(sf::Color::Black);
		std::any_cast<sf::Text&>(velocityText).setCharacterSize(14);


	}

	ProgramModeID handleEvent(const std::optional<sf::Event> eventPtr) override {
		

		//handle events according to setup state
		switch (setupState) {

		case(SetupState::SelectObject):

			//Move Planet to mouse Pos
			if (eventPtr->is<sf::Event::MouseMoved>()) {
				currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), guiView);
				chosenPlanet.setPosition({ currentMousePos.x, currentMousePos.y });
			}

			//Change chosen Planet via arrow keys
			if (eventPtr->is<sf::Event::KeyPressed>()) {
				if (eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down) {
					chosenPlanetEnum = static_cast<PlanetEnum>((chosenPlanetEnum + 1) % PlanetCount); //cool use of wraparound behaviour
					chosenPlanet = planetContainer[chosenPlanetEnum];	
				}
				else if (eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up) {
					chosenPlanetEnum = static_cast<PlanetEnum>((static_cast<int>(chosenPlanetEnum) - 1 + PlanetCount) % PlanetCount);
					chosenPlanet = planetContainer[chosenPlanetEnum];
				}

				currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), guiView);
				chosenPlanet.setPosition({ currentMousePos.x, currentMousePos.y });

				//Set current line to display chosen Planet
				tableWindow.setLineStrings(chosenPlanetEnum, objectNo + 1);
			}

			
			//Confirm chosen Planet via left mouse key
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				if (eventPtr->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
					currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), guiView);
					chosenPlanet.setPosition({ currentMousePos.x, currentMousePos.y });

					setupState = SetupState::SetVelocity;
					instructionWindow.setTextLineString("Please set the Planets starting velocity!", 0);
				}
			}

			



			break;


		case(SetupState::SetVelocity):
			drawVelocityVector = true;
			drawVelocityText = true;

			//Handle repeating actions
			if (eventPtr->is<sf::Event::MouseMoved>()) {

				sf::Vector2f sfMousePos = { (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.x, (float)eventPtr->getIf<sf::Event::MouseMoved>()->position.y };
				std::vector<double> planetPos = chosenPlanet.getPosition();
				sf::Vector2f sfPlanetPos{ (float)planetPos[0], (float)planetPos[1] };
				sfDistanceVector = sfMousePos - sfPlanetPos;
				float length = sfDistanceVector.length();

				

				//clamp vectorLength to ensure that simulation stays in observable range
				if (length > 141.42f) {
					sfDistanceVector *= 141.42f / length;
				}

				vectorLength = sfDistanceVector.length();
				if (vectorLength > 0) {
					vectorAngle = sfDistanceVector.angle().asDegrees();
				}
				else
				{
					vectorAngle = 0;
				}

				float speed = length * 0.2f * 1000;
				std::cout << "Speed = " << speed << "\n";
				std::any_cast<sf::Text&>(velocityText).setString(std::to_string(speed) + "km/s");
				std::any_cast<sf::Text&>(velocityText).setPosition(vectorToSfVector(planetPos) + sf::Vector2f{ sfDistanceVector.x * 1.2f, sfDistanceVector.y * 1.2f });
				
			}
			//Handle confirmation action
			//Confirm chosen Planet via left mouse key
			if (eventPtr->is<sf::Event::MouseButtonPressed>()) {
				if (eventPtr->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
					sf::Vector2f scaledVelocity = sfDistanceVector * 0.2f;
					chosenPlanet.setVelocity({scaledVelocity.x, scaledVelocity.y });

					std::cout << "Velocities: " << chosenPlanet.getVelocity()[0] << chosenPlanet.getVelocity()[1] << std::endl;

					float planetSpeed = std::sqrt(sfDistanceVector.x * sfDistanceVector.x + sfDistanceVector.y * sfDistanceVector.y);
					std::ostringstream oss;
					oss << std::fixed << std::setprecision(1) << planetSpeed * 1000 * 0.2;
					tableWindow.setString(oss.str() + " km/s", objectNo + 1, 3);

					
					setupState = SetupState::SetName;
					drawVelocityText = false;
					instructionWindow.setTextLineString("Please enter a name for the Planet!", 0);
					drawInputWindow = true;
				}
			}

			break;


		case(SetupState::SetName):
			if (eventPtr->is<sf::Event::KeyPressed>()) {
				std::optional<std::string> returnedString = inputWindow.handleInputEvent(eventPtr);
				 if (returnedString.has_value()) {
					 tableWindow.setString(returnedString.value(), objectNo + 1, 0);
					 chosenPlanet.setName(returnedString.value());

					 setupState = SetupState::AdditionalObjectRequest;
					 CelestialObject::Builder::finalizeSetup(chosenPlanet);
					 Simulation::Builder::addCelestialObject(simulation, chosenPlanet);

					 inputWindow.setTextLineString("Would you like to add", 0);
					 inputWindow.setTextLineString("another object? [Y/N]", 1);

					 instructionWindow.setTextLineString("You can start the Simulation by pressing [N]", 0);

					 drawVelocityVector = false;
				 }
			}
			
			break;

		case(SetupState::AdditionalObjectRequest):
			
			if (eventPtr->is<sf::Event::KeyPressed>()) {
				if (eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Y) {
					tableWindow.addLine();
					objectNo++;
					tableWindow.autoSetHeigth();
					tableWindow.setLineStrings(chosenPlanetEnum, objectNo + 1);


					inputWindow.setTextLineString("The name must be unique!", 0);
					inputWindow.setTextLineString("", 1);
					inputWindow.setPosition(conf::ui::marginVector + sf::Vector2f{ 0,125 + (float)objectNo * 24.0f });
					setupState = SetupState::SelectObject;

					instructionWindow.setTextLineString("Please select a Planet using the arrow keys!", 0);

					drawInputWindow = false;
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
		
		window.setView(currentView);
		window.draw(chosenPlanet.getObjectDrawable());
		if (drawVelocityVector) {
			drawVector(vectorToSfVector(chosenPlanet.getPosition()), vectorLength, vectorAngle, &window);
		}
		if (drawVelocityText) {
			window.draw(std::any_cast<sf::Text&>(velocityText));
		}
		std::vector<sf::CircleShape> drawablePlanets = simulation.getObjectDrawables();
		for (int i = 0; i < drawablePlanets.size(); i++) {
			window.draw(drawablePlanets[i]);
		}
		
		for (int i = 0; i < simulation.celestialObjectContainer.size(); i++) {
			std::vector<double> vel = simulation.celestialObjectContainer[i].getVelocity();
			sf::Vector2 sfVel = vectorToSfVector(vel);
			float length = std::sqrt(vel[0] * vel[0] + vel[1] * vel[1]) / 0.2;
			float angle = sfVel.angle().asDegrees();
			drawVector(vectorToSfVector(simulation.celestialObjectContainer[i].getPosition()), length, angle, &window);
		}



		window.setView(guiView);
		instructionWindow.draw(&window);
		tableWindow.draw(&window);
		if (drawInputWindow) {
			inputWindow.draw(&window);
		}
		

		window.display();
	}
};

