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

	}
}