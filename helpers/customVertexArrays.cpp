#include "customVertexArrays.h"

sf::VertexArray createQuarterArc(sf::Vector2f centerPosition, float radius, int quadrant, int Verticies, sf::Color color) {
	assert(quadrant >= 1 && quadrant <= 4 && "Error: Quadrant may only be between 1 and 4!");
	assert(Verticies >= 4 && "Error: Not Enough Verticies!");

	const double pi = 3.14159265358979323846;

	double startAngle = 0.0;

	sf::VertexArray arc;

	sf::Vertex startVertex;
	sf::Vertex endVertex;

	switch (quadrant) {
	case 1:
		startVertex.position = { centerPosition.x + radius, centerPosition.y };
		endVertex.position = { centerPosition.x , centerPosition.y + radius };
		startAngle = 0.0;
		break;
	case 2:
		startVertex.position = { centerPosition.x , centerPosition.y + radius };
		endVertex.position = { centerPosition.x - radius, centerPosition.y };
		startAngle = pi / 2.0f;
		break;
	case 3:
		startVertex.position = { centerPosition.x - radius, centerPosition.y };
		endVertex.position = { centerPosition.x , centerPosition.y - radius };
		startAngle = pi;
		break;
	case 4:
		startVertex.position = { centerPosition.x , centerPosition.y - radius };
		endVertex.position = { centerPosition.x + radius, centerPosition.y };
		startAngle = 3.0f * pi / 2.0f;
		break;
	}

	arc.append(startVertex);

	double startingAngle = (quadrant - 1) * (pi / 2);

	for (int i{ 1 }; i <= Verticies - 2; i++) {
		float x = radius * std::cos(startAngle + i * pi / 2.0f / (Verticies - 1));
		float y = radius * std::sin(startAngle + i * pi / 2.0f / (Verticies - 1));
		sf::Vertex v;
		v.position = { centerPosition.x + x, centerPosition.y + y };
		arc.append(v);
	}

	arc.append(endVertex);

	for (int i = 0; i < arc.getVertexCount(); i++) {
		arc[i].color = color;
	}

	return arc;
}

sf::VertexArray createRoundedRect(sf::Vector2f Position, float Length, float Heigth, float LineThickness, float cornerRadius, sf::Color color) {
	
	sf::VertexArray array;
	std::vector<sf::Vector2f> arcCenters{ {Position.x + Length - cornerRadius, Position.y + Heigth - cornerRadius},
										  {Position.x +        + cornerRadius, Position.y + Heigth - cornerRadius},
										  {Position.x +		   + cornerRadius, Position.y +		   + cornerRadius},
										  {Position.x + Length - cornerRadius, Position.y +		   + cornerRadius}
	};

	for (int i = 0; i < 4; i++) {
		sf::VertexArray arc = createQuarterArc(arcCenters[i], cornerRadius, i + 1, 24, color);
		for (int v = 0; v < arc.getVertexCount(); v++) {
			array.append(arc[v]);
		}
		
	}

	return array;
}