#pragma once
#include <SFML/Graphics.hpp>

inline sf::Vector2f rotateAroundPoint(sf::Vector2f pointToRotate, sf::Vector2f centerPoint, float angleGrad) {
    sf::Vector2f rotatingVector = pointToRotate - centerPoint;
    sf::Vector2f rotatedVector;
    sf::Vector2f rotatedPoint;

    //right angle exceptions:
    if (angleGrad == 90.0f || angleGrad == -270.0f) {
        rotatedVector.x = -rotatingVector.y;
        rotatedVector.y = rotatingVector.x;
    }
    else if (angleGrad == 180.0f || angleGrad == -180.0f) {
        rotatedVector.x = -1.0f * rotatingVector.y;
        rotatedVector.y = -1.0 * rotatingVector.x;
    }
    else if (angleGrad == 270.0f || angleGrad == -90.0f) {
        rotatedVector.x = -rotatingVector.y;
        rotatedVector.y = rotatingVector.x;
    }
    else if (angleGrad == 0.0f) {
        return pointToRotate;
    }

    //handling all other angles
    float angleRad = angleGrad * (3.14159265f / 180.0f); // Convert angle from degrees to radians
    float s = std::sin(angleRad);
    float c = std::cos(angleRad);

    rotatedVector.x = rotatingVector.x * c - rotatingVector.y * s;
    rotatedVector.y = rotatingVector.x * s + rotatingVector.y * c;

    rotatedPoint = centerPoint + rotatedVector;

    return rotatedPoint;

}

inline void rotateAroundPoint(sf::VertexArray& vertexArrayToRotate, sf::Vector2f centerPoint, float angleGrad) {
    for (int i = 0; i < vertexArrayToRotate.getVertexCount(); i++) {
        vertexArrayToRotate[i].position = rotateAroundPoint(vertexArrayToRotate[i].position, centerPoint, angleGrad);
    }
}

inline void drawVector(sf::Vector2f startPos, float length, float angleGrad, sf::RenderWindow* window) {
    float lineWidth = 1.5f;
    sf::Color clr(sf::Color::Magenta);

    //Check for negative Length
    if (length < 0) {
        length = abs(length);
        angleGrad += 180;
    }

    //Scale vectorTip When below 100px length
    float vectorTipScaler;
    if (length < 100.0f) {
        vectorTipScaler = (1 - abs(length) / 100.0f) * 1.8;
    }
    else {
        vectorTipScaler = 0;
    }

    //Create VectorTip
    sf::Vector2f endPos;
    endPos.x = startPos.x + length;
    endPos.y = startPos.y;

    sf::VertexArray vectorTip(sf::PrimitiveType::Triangles, 3);
    vectorTip[0].position = sf::Vector2f({ endPos.x, endPos.y });
    vectorTip[1].position = sf::Vector2f({ endPos.x - (10.0f - vectorTipScaler), endPos.y + (5.0f - vectorTipScaler) });
    vectorTip[2].position = sf::Vector2f({ endPos.x - (10.0f - vectorTipScaler), endPos.y - (5.0f - vectorTipScaler) });
    vectorTip[0].color = clr;
    vectorTip[1].color = clr;
    vectorTip[2].color = clr;

    //Create VectorLine
    sf::RectangleShape vectorLine({ length - 1, lineWidth });
    vectorLine.setOrigin({ 0.0f, lineWidth / 2 }); //adjust line origin so it rotation point is inside its long axis
    vectorLine.setPosition(startPos);
    vectorLine.setFillColor(clr);

    //Handle Vector rotation
    rotateAroundPoint(vectorTip, startPos, angleGrad);
    vectorLine.rotate(sf::degrees(angleGrad));

    window->draw(vectorLine);
    window->draw(vectorTip);
}