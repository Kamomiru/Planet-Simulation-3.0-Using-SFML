#include <SFML/Graphics.hpp>
#include "programWindow.h"
#include "modeHandler.h"
#include "simulation.h"


int main()
{   
	initSFMLWindow();

	Simulation sim; //Default Constructor

	CelestialObject earth("earth", constants::earth::mass, constants::earth::radius, { 300.0f,600.0f }, { 10.0,-2.0 }, sf::Color::Blue);
	CelestialObject moon("moon", constants::moon::mass, constants::moon::radius, { 100.0f, 80.0f }, { 4.0,-1.0 }, sf::Color::White);

	Simulation::Builder::addCelestialObject(sim, earth);
	Simulation::Builder::addCelestialObject(sim, moon);
	Simulation::Builder::finalizeSetup(sim);
    
	SimulationTestMode::initializeSimulation(sim); //Initialize static sim variable in SimulationTestMode

	ModeHandler modeHandler(std::make_unique<StartupMode>()); // Start in StartupMode

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->getIf<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::S) {
				modeHandler.setMode(std::make_unique<StartupMode>());
            }

			if (event->getIf<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::D) {
                modeHandler.setMode(std::make_unique<DisplayTestMode>());
			}

            if (event->getIf<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::F) {
                modeHandler.setMode(std::make_unique<SimulationTestMode>());
            }

			modeHandler.handleEvent(*event); //we can use the derefence operator * here to get the actual event. 
            //we could also use following syntax here:
            //modeHandler.handleEvent(event.value())
        }

		modeHandler.render(window);
		modeHandler.update();
		

    }
}
