#include "customVertexArrays.h"


sf::VertexArray createRoundedRectangle(sf::Vector2f Position, float curveRadius, float boxLength, float boxHeigth, sf::Color color, int qualityScaler = 8) {
    double pi{ 3.141592653589793 };
    //TODO: implement asserts

    //Setting up Vertex Array
    int arraySize{ 4 * 4 * qualityScaler + 4 };
    sf::VertexArray array(sf::PrimitiveType::TriangleStrip, arraySize);

    float offsetX{ 0 };
    float offsetY{ 0 };
    int quadrant{ 1 };

    for (int i{ 0 }, angleStep{ 0 }; i < arraySize; i++) {
        if (i == 0 || i == 4 * qualityScaler + 1 || i == 4 * qualityScaler * 2 + 2 || i == 4 * qualityScaler * 3 + 3) { //Quadrant start Values
            //Setting Offsets
            switch (quadrant) {
            case 1:
                offsetX = { 0.0f };
                offsetY = { 0.0f };
                break;
            case 2:
                offsetX = { boxLength - 2 * curveRadius };
                offsetY = { 0.0f };
                break;
            case 3:
                offsetX = { boxLength - 2 * curveRadius };
                offsetY = { boxHeigth - 2 * curveRadius };
                break;
            case 4:
                offsetX = { 0.0f };
                offsetY = { boxHeigth - 2 * curveRadius };
                break;
            }
            //define Quadrant start Point
            array[i].position = sf::Vector2f(-std::cos(2.0f * pi * (((float)angleStep) / (arraySize - 4.0f))) * curveRadius + curveRadius + Position.x + offsetX,
                -std::sin(2.0f * pi * (((float)angleStep) / (arraySize - 4.0f))) * curveRadius + curveRadius + Position.y + offsetY);
            array[i].color = color;
            quadrant++; //Increment quadrant, so the next time we are at a start point the X and Y offsets can be adjusted
        }
        else { //define remaining Quadrand Points
            angleStep++;
            array[i].position = sf::Vector2f(-std::cos(2.0f * pi * (((float)angleStep) / (arraySize - 4.0f))) * curveRadius + curveRadius + Position.x + offsetX,
                -std::sin(2.0f * pi * (((float)angleStep) / (arraySize - 4.0f))) * curveRadius + curveRadius + Position.y + offsetY);
            array[i].color = color;

        }

    }
    return array;
}

sf::VertexArray createRoundedRectangleBorder(sf::Vector2f Position, float curveRadius, float boxLength, float boxHeigth, float lineThickness, sf::Color color, int qualityScaler = 8) {
    //TODO: implement asserts

    //Create Inner and outer vertex Arrays
    sf::VertexArray outerRect = createRoundedRectangle(Position, curveRadius, boxLength, boxHeigth, color);
    sf::VertexArray innerRect = createRoundedRectangle({ Position.x + lineThickness, Position.y + lineThickness }, curveRadius - lineThickness, boxLength - 2 * lineThickness, boxHeigth - 2 * lineThickness, color);

    //Create border vertex Array
    sf::VertexArray border(sf::PrimitiveType::TriangleStrip, outerRect.getVertexCount() * 2);

    //Combine vertex Arrays into one suitable for sf::PrimitiveType::TriangleStrip
    for (int i{ 0 }, j{ 0 }; i < outerRect.getVertexCount() * 2;) {
        border[i] = outerRect[j];
        i++;
        border[i] = innerRect[j];
        i++, j++;
    }
    //Add first two points again to close Border
    border.append(sf::Vertex{ {Position.x, Position.y + curveRadius}, color });
    border.append(sf::Vertex{ {Position.x + lineThickness, Position.y + curveRadius}, color });

    return border;
}