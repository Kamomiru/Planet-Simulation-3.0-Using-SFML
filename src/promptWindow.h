#pragma once
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"
#include "../helpers/stringHelper.h"
#include "../src/programConfig.h"
#include <string>
#include <vector>
#include <any>


class PromptWindow {
protected:
	sf::Vector2f position;

	//Window Graphics variables
	sf::Vector2f size;
	float cornerRadius = 10.0f;
	float borderLineThickness = 5.0f;
	float padding = 16.0f;
	sf::Color windowBackgroundColor = conf::ui::darkGrey;
	sf::Color windowBorderColor = conf::ui::darkDarkGrey;
	sf::VertexArray backgroundVertexArray;
	sf::VertexArray borderVertexArray;

	//Text variables
	std::vector<sf::Text> textLinesVector;
	int fontSize = 18;
	int lineDistance = 8.0f;
	sf::Color fontColor = sf::Color::White;
	sf::Font* fontPtr = conf::font::CascadiaRegularPtr.get();
	sf::Vector2f textSizeLineOne;

public:
	PromptWindow() = default;

	PromptWindow(sf::Vector2f Position, sf::Vector2f Size, std::vector<std::string> TextLines) :
		position(Position), size(Size) {

		for (std::string str : TextLines) {
			sf::Text textLine(*fontPtr);
			textLine.setCharacterSize(fontSize);
			textLine.setFillColor(fontColor);
			textLine.setString(str);
			textLine.setPosition(position);
			textLinesVector.push_back(textLine);
		}

		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);
	}

	void autoTextSpacing() {
		assert(!textLinesVector.empty() && "Error: textLinesVector is empty. Cannot automatically space text!");

		//float textHeigth = textLinesVector[0].getGlobalBounds().size.y;
		sf::Vector2f textStartPos = position + sf::Vector2f({ padding, padding});
		
		float fontVerticalOffset = 0;
		switch(fontSize) {
		case(18):
			fontVerticalOffset = 4;
			break;
		default:
			assert(false && "Error: vertical font offset for given font size has not been defined yet!");
		}

		//int counter = 0;
		float horizontalTextIncrement = 0;
		for (sf::Text& text : textLinesVector) {
			//textHeigth = text.getGlobalBounds().size.y;
			text.setPosition(textStartPos + sf::Vector2f{0.0, horizontalTextIncrement});
			text.setOrigin(sf::Vector2f({ 0.0f, fontVerticalOffset })); //sets Text Origin so that the left top pixel of the text is its origin
			
			//std::cout << "Line " << counter << " Text Heigth = " << textHeigth << " Font Size = " << fontSize << " Text Origin = " << " Origin X = " << text.getOrigin().x << " Origin Y = " << text.getOrigin().y << "\n";
			horizontalTextIncrement += fontSize + lineDistance;
			//counter++;
		}
	}

	void autoWindowSpacing() {
		assert(!textLinesVector.empty() && "Error: textLinesVector is empty. Cannot automatically space Window!");
		autoTextSpacing();

		int lines = textLinesVector.size();

		float maxLength = 0;
		for (sf::Text& text : textLinesVector) {
			float length = text.getGlobalBounds().size.x;
			if (length > maxLength) {
				maxLength = length;
			}
		}

		size.x = 2 * padding + maxLength;
		size.y = 2 * padding + (lines - 1) * lineDistance + lines * fontSize;
		

		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);

	}

	void draw(sf::RenderWindow* renderWindow, bool drawBoundaries = false) {
		renderWindow->draw(backgroundVertexArray);
		renderWindow->draw(borderVertexArray);

		if (drawBoundaries) {
			drawTextBoundaries(renderWindow);
		}
		
		for (sf::Text text : textLinesVector) {
			renderWindow->draw(text);
		}

		////Draw Window Origin
		//sf::Vector2f originPosition = position;
		//sf::VertexArray array;
		//array.append(sf::Vertex{ originPosition });
		//array.append(sf::Vertex{ originPosition - sf::Vector2f{20.0f, 20.0f} });
		//array[0].color = sf::Color::Red;
		//array[1].color = sf::Color::Red;
		//array.setPrimitiveType(sf::PrimitiveType::Lines);

		//renderWindow->draw(array);
	}

	void drawTextBoundaries(sf::RenderWindow* renderWindow) {
		for (sf::Text text : textLinesVector) {
			sf::RectangleShape boundsRect;
			boundsRect.setSize(text.getGlobalBounds().size);
			boundsRect.setPosition(text.getGlobalBounds().position);
			//boundsRect.setOutlineThickness(1.0f);
			boundsRect.setOutlineColor(sf::Color::Red);
			boundsRect.setFillColor(sf::Color::Red);
			
			//Draw Text Origins
			sf::Vector2f originPosition = text.getPosition();
			sf::VertexArray array;
			array.append(sf::Vertex{ originPosition });
			array.append(sf::Vertex{ originPosition - sf::Vector2f{20.0f, 20.0f} });
			array[0].color = sf::Color::Red;
			array[1].color = sf::Color::Red;
			array.setPrimitiveType(sf::PrimitiveType::Lines);

			renderWindow->draw(array);

			renderWindow->draw(boundsRect);
		}
	}
};


