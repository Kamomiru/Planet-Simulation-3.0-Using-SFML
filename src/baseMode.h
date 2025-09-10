#pragma once

class BaseMode {
public:

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual ~BaseMode() = default;

	
};

