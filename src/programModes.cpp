#include "programModes.h"
#include <SFML/Graphics.hpp>

class MenuMode : public BaseMode {
private:
	sf::RectangleShape rectangle;

public:
	MenuMode() {
		rectangle.setSize(sf::Vector2f(200.f, 100.f));
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setPosition({800.0f, 600.0f}); // Center of a 1920x1080 view
	}

	void handleEvent() override {
		// handle events specific to menu mode
	}
	void render(sf::RenderWindow& window) override {
		window.clear(sf::Color::White);
		window.draw(rectangle);
		window.display();
	}

};