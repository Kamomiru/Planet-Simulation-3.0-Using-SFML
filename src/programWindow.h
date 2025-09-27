#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "programConfig.h"

inline sf::VideoMode desktop;
inline sf::View currentView;
inline sf::View guiView;
inline sf::RenderWindow window;
inline sf::Vector2f windowCenter;
inline sf::Vector2f windowSize;

inline void initSFMLWindow() {
	//setup MSAA --> smoother Graphics at the cost of performance
	sf::ContextSettings settings;
	settings.antiAliasingLevel = conf::graphics::antialiasingLevel;

	desktop = sf::VideoMode::getDesktopMode();

	//Setup User Interface View -> the view the GUI is drawn in. So everything that stayes on a fixed screen position
	guiView = sf::View((sf::FloatRect({ 0.0f, 0.0f }, static_cast<sf::Vector2f>(desktop.size))));
	
	//Setup current View -> the view the simulation and any objects that are supposed to be moved are being drawn in
	currentView = sf::View((sf::FloatRect({ 0.0f, 0.0f }, static_cast<sf::Vector2f>(desktop.size))));

	//initializing window object
	window = sf::RenderWindow(desktop, "PlanetSimulation", sf::State::Windowed, settings);
	window.setFramerateLimit(conf::graphics::frameRateLimit);

	//initialize windowSize
	windowSize = static_cast<sf::Vector2f>(desktop.size);

	//initialize windowCenter
	windowCenter = windowSize / 2.0f;
	std::cout << "Window Center: " << windowCenter.x << ", " << windowCenter.y << std::endl;

	
}

//TODO: Check if updateView should be defined here?

//inline void updateView(bool mouseDragged, sf::Vector2f& currentMousePos, sf::RenderWindow& window, sf::View& view, sf::Vector2f& lastMousePos) {
//	//Camera Movement Logic
//	if (mouseDragged) {
//		currentMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
//		sf::Vector2f delta = lastMousePos - currentMousePos;
//		if (true) {
//			view.move(delta);
//		}
//		lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
//	}
//}