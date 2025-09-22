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
	}

	void drawTextBoundaries(sf::RenderWindow* renderWindow) {
		for (sf::Text text : textLinesVector) {
			sf::RectangleShape boundsRect;
			boundsRect.setSize(text.getGlobalBounds().size);
			boundsRect.setPosition(text.getGlobalBounds().position);
			//boundsRect.setOutlineThickness(1.0f);
			boundsRect.setOutlineColor(sf::Color::Red);
			boundsRect.setFillColor(sf::Color::Red);
			
			/*Draw Text Origins
			sf::Vector2f originPosition = text.getPosition();
			sf::VertexArray array;
			array.append(sf::Vertex{ originPosition });
			array.append(sf::Vertex{ originPosition - sf::Vector2f{20.0f, 20.0f} });
			array[0].color = sf::Color::Red;
			array[1].color = sf::Color::Red;
			array.setPrimitiveType(sf::PrimitiveType::Lines);

			renderWindow->draw(array);
			*/

			renderWindow->draw(boundsRect);
		}
	}
};

class InputWindow : public PromptWindow {
protected:
	int inputLine = 1;
	std::string inputString;
	double numericalInput = 0;

public:
	InputWindow() = default;

	InputWindow(sf::Vector2f Position, sf::Vector2f Size, std::vector<std::string> TextLines, int InputLine) : PromptWindow() { //Calls default constructor so we can "override" InputWindow(...)

		position = Position;
		size = Size;
		inputLine = InputLine;
		
		//Create sf::Text Objects
		for (int i = 0; i < TextLines.size(); i++) {
			sf::Text textLine(*fontPtr);
			textLine.setCharacterSize(fontSize);
			textLine.setFillColor(fontColor);
			textLine.setPosition(position);
			if (i == inputLine) {
				textLine.setString("");
				textLinesVector.push_back(textLine);
			}
			textLine.setString(TextLines[i]);
			textLinesVector.push_back(textLine);
		}

		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);

	}

	void handleInputEvent(const std::optional<sf::Event> eventPtr, bool numberInputOnly = false) {
		assert(eventPtr->is<sf::Event::KeyPressed>() && "Error: Input Window got passed an event that is not KeyPressed! Only pass sf::Event::KeyPressed!");
		
		int inputCode = (int)eventPtr->getIf<sf::Event::KeyPressed>()->code;
		char inputChar = '?';

		//std::cout << inputCode << "\n";
		
		if (!numberInputOnly) {
			switch (inputCode) {
			case 0:  inputChar = 'a'; break;
			case 1:  inputChar = 'b'; break;
			case 2:  inputChar = 'c'; break;
			case 3:  inputChar = 'd'; break;
			case 4:  inputChar = 'e'; break;
			case 5:  inputChar = 'f'; break;
			case 6:  inputChar = 'g'; break;
			case 7:  inputChar = 'h'; break;
			case 8:  inputChar = 'i'; break;
			case 9:  inputChar = 'j'; break;
			case 10: inputChar = 'k'; break;
			case 11: inputChar = 'l'; break;
			case 12: inputChar = 'm'; break;
			case 13: inputChar = 'n'; break;
			case 14: inputChar = 'o'; break;
			case 15: inputChar = 'p'; break;
			case 16: inputChar = 'q'; break;
			case 17: inputChar = 'r'; break;
			case 18: inputChar = 's'; break;
			case 19: inputChar = 't'; break;
			case 20: inputChar = 'u'; break;
			case 21: inputChar = 'v'; break;
			case 22: inputChar = 'w'; break;
			case 23: inputChar = 'x'; break;
			case 24: inputChar = 'y'; break;
			case 25: inputChar = 'z'; break;
			case 26: inputChar = '0'; break;
			case 27: inputChar = '1'; break;
			case 28: inputChar = '2'; break;
			case 29: inputChar = '3'; break;
			case 30: inputChar = '4'; break;
			case 31: inputChar = '5'; break;
			case 32: inputChar = '6'; break;
			case 33: inputChar = '7'; break;
			case 34: inputChar = '8'; break;
			case 35: inputChar = '9'; break;

			case 39: inputChar = 'ü'; break;
			case 51: inputChar = 'ä'; break;
			case 57: inputChar = ' '; break;
			case 54: inputChar = 'ö'; break;

			case 59:
				if (!inputString.empty()) { inputString.pop_back(); }; break;
			default: inputChar = '?'; break; // fallback
			}
		}
		else {
			switch (inputCode) {
			case 26: inputChar = '0'; break;
			case 27: inputChar = '1'; break;
			case 28: inputChar = '2'; break;
			case 29: inputChar = '3'; break;
			case 30: inputChar = '4'; break;
			case 31: inputChar = '5'; break;
			case 32: inputChar = '6'; break;
			case 33: inputChar = '7'; break;
			case 34: inputChar = '8'; break;
			case 35: inputChar = '9'; break;

			case 59:
				if (!inputString.empty()) { inputString.pop_back(); }; break;

			default: inputChar = '?'; break; // fallback
			}
			
		}

		if (inputChar != '?') {
			inputString += inputChar;
			if (numberInputOnly) {
				numericalInput = std::stod(inputString);
				std::cout << numericalInput << "\n";
			}
		}
		textLinesVector[inputLine].setString(inputString);
		getNumericalInput();
	}

	std::string getInput() {
		return inputString;
	}

	double getNumericalInput() {
		assert(numericalInput != 0 && "Error: there is no numerical Input yet!");
		
		return numericalInput;
		
	}

};


