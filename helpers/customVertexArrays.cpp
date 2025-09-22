#include "customVertexArrays.h"

sf::VertexArray createQuarterArc(sf::Vector2f centerPosition, float radius, int quadrant, int Verticies, sf::Color Color) {
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
		arc[i].color = Color;
	}

	return arc;
}

sf::VertexArray createRoundedRect(sf::Vector2f Position, float Length, float Heigth, float CornerRadius, sf::Color Color) {
	
	sf::VertexArray array;
	std::vector<sf::Vector2f> arcCenters{ {Position.x + Length - CornerRadius, Position.y + Heigth - CornerRadius},
										  {Position.x +        + CornerRadius, Position.y + Heigth - CornerRadius},
										  {Position.x +		   + CornerRadius, Position.y +		   + CornerRadius},
										  {Position.x + Length - CornerRadius, Position.y +		   + CornerRadius}
	};

	for (int i = 0; i < 4; i++) {
		sf::VertexArray arc = createQuarterArc(arcCenters[i], CornerRadius, i + 1, 24, Color);
		for (int v = 0; v < arc.getVertexCount(); v++) {
			array.append(arc[v]);
		}
		
	}
	array.setPrimitiveType(sf::PrimitiveType::TriangleFan);

	return array;
}

sf::VertexArray createRoundedRectBorder(sf::Vector2f Position, float Length, float Heigth, float CornerRadius, float LineThickness, sf::Color Color) {
	sf::VertexArray outsideArray;
	sf::VertexArray insideArray;
	sf::VertexArray array;
	std::vector<sf::Vector2f> arcCenters{ {Position.x + Length - CornerRadius, Position.y + Heigth - CornerRadius},
										  {Position.x +		   + CornerRadius, Position.y + Heigth - CornerRadius},
										  {Position.x +        + CornerRadius, Position.y +        + CornerRadius},
										  {Position.x + Length - CornerRadius, Position.y +        + CornerRadius}
	};

	for (int i = 0; i < 4; i++) {
		sf::VertexArray arc = createQuarterArc(arcCenters[i], CornerRadius, i + 1, 48, Color);
		for (int v = 0; v < arc.getVertexCount(); v++) {
			outsideArray.append(arc[v]);
		}
	}

	for (int i = 0; i < 4; i++) {
		sf::VertexArray arc = createQuarterArc(arcCenters[i], CornerRadius - LineThickness, i + 1, 48, Color);
		for (int v = 0; v < arc.getVertexCount(); v++) {
			insideArray.append(arc[v]);
		}
	}

	for (int v = 0; v < outsideArray.getVertexCount(); v++) {
		array.append(outsideArray[v]);
		array.append(insideArray[v]);
	}

	array.append(outsideArray[0]);
	array.append(insideArray[1]);

	array.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	
	return array;
}