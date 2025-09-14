#pragma once
#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <vector>

sf::VertexArray createQuarterArc(sf::Vector2f centerPosition, float radius, int quadrant, int Verticies, sf::Color color);

sf::VertexArray createRoundedRect(sf::Vector2f Position, float Length, float Heigth, float LineThickness, float cornerRadius, sf::Color color);
