#pragma once
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"



class RoundedRect : public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray m_vertices;
    sf::Texture     m_texture;
    sf::Vector2f position;
    float length = 0;
    float heigth = 0;
    float lineThickness = 0;
    float cornerRadius = 0;
    sf::Color color;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

        // apply the texture
        states.texture = &m_texture;

        // you may also override states.shader or states.blendMode if you want

        // draw the vertex array
        target.draw(m_vertices, states);
    }

public:
    RoundedRect() = default;

    RoundedRect(sf::Vector2f Position, float Length, float Heigth, float CornerRadius, sf::Color Color)
        : position(Position), length(Length), heigth(Heigth), cornerRadius(CornerRadius), color(Color) {
        m_vertices = createRoundedRect(position, length, heigth, cornerRadius, color);
        m_vertices.setPrimitiveType(sf::PrimitiveType::TriangleFan);
    }

    void setOriginToCenter() {
        sf::Vector2f center({ length / 2.0f , heigth / 2.0f });
        this->setOrigin(center);
    }


};




