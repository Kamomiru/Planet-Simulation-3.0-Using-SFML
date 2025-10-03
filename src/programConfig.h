#pragma once
#include <SFML/Graphics.hpp>

namespace conf {
	namespace graphics {
		constexpr unsigned int antialiasingLevel = 8; // MSAA level
		constexpr unsigned int frameRateLimit = 144; // Frame rate limit
	}
	namespace font {
		constexpr unsigned int defaultFontSize = 14; // Default font size

		

		std::unique_ptr<sf::Font> loadFont(std::string s) {
			auto font = std::make_unique<sf::Font>();
			std::string path = "../resources/" + s;
			if (!font->openFromFile(path)) {
				throw std::runtime_error("Failed to load default font from path: " + path);
			}
			return font;
		}
		
		std::unique_ptr<sf::Font> RobotoMediumPtr = loadFont("Roboto-Medium.ttf");
		std::unique_ptr<sf::Font> CascadiaRegularPtr = loadFont("CascadiaCode-Regular.ttf");
		std::unique_ptr<sf::Font> CascadiaSemiBoldPtr = loadFont("CascadiaCode-SemiBold.ttf");

	}
	namespace ui {
		sf::Color programBackgroundColor = sf::Color::White;
		sf::Color windowBackgroundColor(38, 45, 53, 255);
		sf::Color windowBorderColor(0, 0, 0, 255);
		sf::Color grey(137, 137, 137, 255);
		sf::Color darkGrey(53, 53, 53, 255);
		sf::Color darkDarkGrey(38, 38, 38, 255);

		float generalSidePadding = 48.0f;
		float generalSideMargin = 24.0f;
		sf::Vector2f marginVector{ generalSideMargin, generalSideMargin };

		uint16_t standardFontSizeMedium = 16;
	}
}