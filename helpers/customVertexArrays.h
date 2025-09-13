#pragma once
#include <SFML/Graphics.hpp>
#include <cassert>

sf::VertexArray createRoundedRectangle(sf::Vector2f Position, float curveRadius, float boxLength, float boxHeigth, sf::Color color, int qualityScaler);
sf::VertexArray createRoundedRectangleBorder(sf::Vector2f Position, float curveRadius, float boxLength, float boxHeigth, float lineThickness, sf::Color color, int qualityScaler);
