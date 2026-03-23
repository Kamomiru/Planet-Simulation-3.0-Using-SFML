#include <SFML/Graphics.hpp>
#include "programWindow.h"
#include "../modes/modeHandler.h"
#include "simulation.h"
#include "programConfig.h"


int main()
{   
	initSFMLWindow();
	
	ModeHandler modeHandler(std::make_unique<StartupMode>()); // Set Start Mode

    while (window.isOpen())
    {
        while (const std::optional eventPtr = window.pollEvent())
        {
            if (eventPtr->is<sf::Event::Closed>())
            {
                window.close();
            }

			modeHandler.handleEvent(eventPtr); //we can use the derefence operator * here to get the actual eventPtr. 
            //we could also use following syntax here:
            //modeHandler.handleEvent(eventPtr.value())
        }

		modeHandler.render(window);
		modeHandler.update();

    }
}
