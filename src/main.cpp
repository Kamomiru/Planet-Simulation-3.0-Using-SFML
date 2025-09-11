#include <SFML/Graphics.hpp>
#include "programWindow.h"
#include "modeHandler.h"


int main()
{   
	initSFMLWindow();

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

			modeHandler.handleEvent(*event); //we can use the derefence operator * here to get the actual event. 
            //we could also use following syntax here:
            //modeHandler.handleEvent(event.value())
        }

		modeHandler.render(window);

		std::cout << "Current Mode ID: " << static_cast<int>(modeHandler.getCurrentModeID()) << std::endl;

    }
}
