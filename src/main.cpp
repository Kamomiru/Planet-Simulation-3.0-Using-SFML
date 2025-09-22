#include <SFML/Graphics.hpp>
#include "programWindow.h"
#include "../modes/modeHandler.h"
#include "simulation.h"
#include "programConfig.h"


int main()
{   
	initSFMLWindow();
	
	ModeHandler modeHandler(std::make_unique<DisplayTestMode>()); // Set Start Mode

	Simulation sim; //Default Constructor

	CelestialObject earth("earth", constants::earth::mass, constants::earth::radius, { 300.0f,600.0f }, { 10.0,-2.0 }, sf::Color::Blue);
	CelestialObject moon("moon", constants::moon::mass, constants::moon::radius, { 100.0f, 80.0f }, { 4.0,-1.0 }, sf::Color::White);
	CelestialObject earth2("earth2", constants::earth::mass, constants::earth::radius, { 800.0f,300.0f }, { -6.0,3.0 }, sf::Color::Red);

	Simulation::Builder::addCelestialObject(sim, earth);
	Simulation::Builder::addCelestialObject(sim, moon);
	Simulation::Builder::addCelestialObject(sim, earth2);
	Simulation::Builder::finalizeSetup(sim);
    
	SimulationTestMode::initializeSimulation(sim); //Initialize static sim variable in SimulationTestMode

    while (window.isOpen())
    {
        while (const std::optional eventPtr = window.pollEvent())
        {
            if (eventPtr->is<sf::Event::Closed>())
            {
                window.close();
            }

			//Switch Program Modes on S, D and F
            /*if (eventPtr->getIf<sf::Event::KeyPressed>() && eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) {
				modeHandler.setMode(std::make_unique<StartupMode>());
            }

			if (eventPtr->getIf<sf::Event::KeyPressed>() && eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D) {
                modeHandler.setMode(std::make_unique<DisplayTestMode>());
			}

            if (eventPtr->getIf<sf::Event::KeyPressed>() && eventPtr->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::F) {
                modeHandler.setMode(std::make_unique<SimulationTestMode>());
            }*/

			modeHandler.handleEvent(eventPtr); //we can use the derefence operator * here to get the actual eventPtr. 
            //we could also use following syntax here:
            //modeHandler.handleEvent(eventPtr.value())
        }

		modeHandler.render(window);
		modeHandler.update();
		

    }
}
