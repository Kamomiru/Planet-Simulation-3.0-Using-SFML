#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

inline sf::VideoMode desktop;
inline sf::View view;
inline sf::View viewGui;
inline sf::RenderWindow window;
inline sf::Vector2f windowCenter;

//Setting up Variables for View Movement
inline bool mouseDragged = false;
inline sf::Vector2f lastMousePos;
inline sf::Vector2f currentMousePos;

inline void initSFMLWindow() {
	//setup MSAA --> smoother Graphics at the cost of performance
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	//Setup Standard View
	view = sf::View({ 860.f, 540.f }, { 1920.0f, 1080.0f });

	//Setup User Interface View
	desktop = sf::VideoMode::getDesktopMode();
	viewGui = sf::View((sf::FloatRect({ 0.0f, 0.0f }, static_cast<sf::Vector2f>(desktop.size))));

	//initializing window object
	window = sf::RenderWindow(desktop, "PlanetSimulation", sf::State::Windowed, settings);
	window.setFramerateLimit(144);

	//initialize windowCenter
	windowCenter = static_cast<sf::Vector2f>(desktop.size) / 2.0f;
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