#pragma once
#include <SFML/Graphics.hpp>
#include "../helpers/customVertexArrays.h"
#include "../helpers/stringHelper.h"
#include "../src/programConfig.h"
#include "../src/PlanetEnum.h"
#include "constants.h"
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
	int fontSize = 14;
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

	//Spaces text automatically within window borders
	void autoTextSpacing() {
		assert(!textLinesVector.empty() && "Error: textLinesVector is empty. Cannot automatically space text!");

		//float textHeigth = textLinesVector[0].getGlobalBounds().size.y;
		sf::Vector2f textStartPos = position + sf::Vector2f({ padding, padding});
		
		float fontVerticalOffset = 0;
		switch(fontSize) {
		case(18):
			fontVerticalOffset = 4;
			break;
		case(14):
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

	//Automatically spaces Text and Window.
	//You can also opt by only spacing Text automatically with autoTextSpacing()!
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

	void setTextLineString(std::string str, int lineNo) {
		assert(!(lineNo < 0) && "Error: Given line number is below 0!");
		assert((lineNo < textLinesVector.size()) && "Error: Given line number must be smaller than textLineVector.size()!");

		textLinesVector[lineNo].setString(str);
	}

	void eraseTextLine(int lineNo) {
		assert(!(lineNo < 0) && "Error: Given line number is below 0!");
		assert((lineNo < textLinesVector.size()) && "Error: Given line number must be smaller than textLineVector.size()!");

		textLinesVector.erase(textLinesVector.begin() + lineNo);
	}

	void setPosition(sf::Vector2f Position) {
		position = Position;
		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);
		autoTextSpacing();
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
		for (std::string str : TextLines) {
			sf::Text text(*fontPtr);
			text.setCharacterSize(fontSize);
			text.setFillColor(fontColor);
			text.setPosition(position);
			text.setString(str);
			textLinesVector.push_back(text);
		}
		//Create sf::Text Object as placeholer for Input Line

		sf::Text text(*fontPtr);
		text.setCharacterSize(fontSize);
		text.setFillColor(fontColor);
		text.setPosition(position);
		text.setString("");
		textLinesVector.insert(textLinesVector.begin() + inputLine, text);

		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);

		std::cout << textLinesVector.size() << "\n";
	}

	std::optional<std::string> handleInputEvent(const std::optional<sf::Event> eventPtr, bool numberInputOnly = false) {
		assert(eventPtr->is<sf::Event::KeyPressed>() && "Error: Input Window got passed an event that is not KeyPressed! Only pass sf::Event::KeyPressed!");
		
		bool inputConfirmed = false;
		int inputCode = (int)eventPtr->getIf<sf::Event::KeyPressed>()->code;
		char inputChar = '?';

		std::cout << inputCode << "\n";

		//convert inputCode to inputChar
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

			case 48: inputChar = 'ü'; break;
			case 51: inputChar = 'ä'; break;
			case 57: inputChar = ' '; break;
			case 54: inputChar = 'ö'; break;

			default: inputChar = '?'; break; // fallback
			}
		}
		//Convert inputCode to inputChar -> numerical values only!
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

			default: inputChar = '?'; break; // fallback
			}
			
		}

		//Check for backspace
		if (inputCode == 59 && !inputString.empty()) {
			inputString.pop_back();
		}
		//Check for enter
		else if (inputCode == 58 && !inputString.empty()) {
			inputConfirmed = true;
		}

		//Add computed inputChar to inputString
		if (inputChar != '?') {
			inputString += inputChar;
			if (numberInputOnly) {
				numericalInput = std::stod(inputString);
				std::cout << numericalInput << "\n";
			}
		}

		//Update inputLines string so it correctly shows what is being typed on screen
		textLinesVector[inputLine].setString(inputString);
		
		if (inputConfirmed) {
			std::string output = inputString; //Copy initialization so when inputString is deleted/changed output stays the same

			inputString = "";
			textLinesVector[inputLine].setString(inputString);

			return output;
		}


		//Return a pointer not holding a value
		return std::nullopt;
	
	}

	std::string getInput() {
		return inputString;
	}

	double getNumericalInput() {
		return numericalInput;
		
	}

	void resetInput() {
		inputString = "";
		textLinesVector[inputLine].setString("");
		std::cout << "Input String has been reset!" << "\n";
	}

};



class ObjectTableWindow {
protected:
	sf::Vector2f position;

	//Window Graphics variables
	sf::Vector2f size;
	float cornerRadius = 10.0f;
	float borderLineThickness = 5.0f;
	float padding = 16.0f;
	sf::Vector2f paddingVector{ padding,padding };
	sf::Color windowBackgroundColor = conf::ui::darkGrey;
	sf::Color windowBorderColor = conf::ui::darkDarkGrey;
	sf::VertexArray backgroundVertexArray;
	sf::VertexArray borderVertexArray;

	//Text variables
	//std::vector<sf::Text> textLinesVector;
	std::vector<std::vector<sf::Text>> sfTextContainer; //Row x Column
	int fontSize = 14;
	int lineDistance = 8.0f;
	int columnDistance = 8.0f;
	int lineNo = 0;
	int lineLength = 100;
	sf::Color fontColor = sf::Color::White;
	sf::Font* fontPtr = conf::font::CascadiaRegularPtr.get();
	sf::Vector2f textSizeLineOne;

	std::vector<std::string> names;
	std::vector<float> masses;
	std::vector<float> radii;
	std::vector<float> velocities;

public:
	ObjectTableWindow() = default;

	ObjectTableWindow(sf::Vector2f Position, sf::Vector2f Size) :
		position(Position), size(Size) {

		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);

		std::vector<sf::Text> row;
		int columnNo = 0;

		sf::Text nameTxt(*fontPtr, "Name", fontSize);
		nameTxt.setFillColor(fontColor);
		nameTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,0.0f });
		row.push_back(nameTxt);

		columnNo++;

		sf::Text massTxt(*fontPtr, "Mass", fontSize);
		massTxt.setFillColor(fontColor);
		massTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,0.0f });
		row.push_back(massTxt);


		columnNo++;

		sf::Text radiusTxt(*fontPtr, "Radius", fontSize);
		radiusTxt.setFillColor(fontColor);
		radiusTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,0.0f });
		row.push_back(radiusTxt);


		columnNo++;

		sf::Text velocityText(*fontPtr, "Velocity", fontSize);
		velocityText.setFillColor(fontColor);
		velocityText.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,0.0f });
		row.push_back(velocityText);

		sfTextContainer.push_back(row);
		lineNo++;
	}

	void appendLine(std::string Name, float Mass, float Radius, float Velocity) {
		std::vector<sf::Text> row;
		int columnNo = 0;

		

		sf::Text nameTxt(*fontPtr);
		nameTxt.setFillColor(fontColor);
		nameTxt.setString(Name);
		nameTxt.setCharacterSize(fontSize);
		nameTxt.setPosition(position + paddingVector + sf::Vector2f{ 0.0f,(float)lineNo * (fontSize + lineDistance) });

		row.push_back(nameTxt);
		columnNo++;

		sf::Text massTxt(*fontPtr);
		massTxt.setFillColor(fontColor);
		massTxt.setCharacterSize(fontSize);
		massTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		std::ostringstream massString;
		massString << std::scientific << std::setprecision(2) << Mass << " kg?";
		massTxt.setString(massString.str());
		
		row.push_back(massTxt);
		columnNo++;

		sf::Text radiusTxt(*fontPtr);
		radiusTxt.setFillColor(fontColor);
		radiusTxt.setCharacterSize(fontSize);
		radiusTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		std::ostringstream radiusString;
		radiusString << std::scientific << std::setprecision(2) << Radius << " m?";
		radiusTxt.setString(radiusString.str());

		row.push_back(radiusTxt);
		columnNo++;

		sf::Text velocityTxt(*fontPtr);
		velocityTxt.setFillColor(fontColor);
		velocityTxt.setCharacterSize(fontSize);
		velocityTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		std::ostringstream velocityString;
		velocityString << std::scientific << std::setprecision(2) << Velocity << " u?";
		velocityTxt.setString(velocityString.str());

		row.push_back(velocityTxt);
		

		sfTextContainer.push_back(row);
		lineNo++;

		autoSetHeigth();

	}

	void addLine() {
		std::vector<sf::Text> row;

		int columnNo = 0;

		sf::Text nameTxt(*fontPtr);
		nameTxt.setFillColor(fontColor);
		nameTxt.setString("");
		nameTxt.setCharacterSize(fontSize);
		nameTxt.setPosition(position + paddingVector + sf::Vector2f{ 0.0f,(float)lineNo * (fontSize + lineDistance) });

		row.push_back(nameTxt);
		columnNo++;


		sf::Text massTxt(*fontPtr);
		massTxt.setFillColor(fontColor);
		massTxt.setCharacterSize(fontSize);
		massTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		massTxt.setString("");

		row.push_back(massTxt);
		columnNo++;


		sf::Text radiusTxt(*fontPtr);
		radiusTxt.setFillColor(fontColor);
		radiusTxt.setCharacterSize(fontSize);
		radiusTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		radiusTxt.setString("");

		row.push_back(radiusTxt);
		columnNo++;

		sf::Text velocityTxt(*fontPtr);
		velocityTxt.setFillColor(fontColor);
		velocityTxt.setCharacterSize(fontSize);
		velocityTxt.setPosition(position + paddingVector + sf::Vector2f{ (lineLength + columnDistance) * (float)columnNo,(float)lineNo * (fontSize + lineDistance) });
		velocityTxt.setString("");

		row.push_back(velocityTxt);


		sfTextContainer.push_back(row);
		lineNo++;

		autoSetHeigth();
	}

	void setLineStrings(PlanetEnum planet, int line) {
		assert(line > 0 && "Error: Can only set line strings of lines > 1!");
		assert(line < sfTextContainer.size() && "Error: line is out of vector range of sfTextContainer!");

		struct PlanetData {
			std::string Name;
			float Mass;
			float Radius;
			float Velocity;
		};

		PlanetData planetData;

		switch (planet) {
		case Earth:
			planetData.Name = "Earth";
			planetData.Mass = constants::earth::mass;
			planetData.Radius = constants::earth::radius;
			planetData.Velocity = 0;
			break;

		case Moon:
			planetData.Name = "Moon";
			planetData.Mass = constants::moon::mass;
			planetData.Radius = constants::moon::radius;
			planetData.Velocity = 0;
			break;

		case Mars:
			planetData.Name = "Mars";
			planetData.Mass = constants::mars::mass;
			planetData.Radius = constants::mars::radius;
			planetData.Velocity = 0;
			break;

		case Jupiter:
			planetData.Name = "Jupiter";
			planetData.Mass = constants::jupiter::mass;
			planetData.Radius = constants::jupiter::radius;
			planetData.Velocity = 0;
			break;

		case Saturn:
			planetData.Name = "Saturn";
			planetData.Mass = constants::saturn::mass;
			planetData.Radius = constants::saturn::radius;
			planetData.Velocity = 0;
			break;

		case Uranus:
			planetData.Name = "Uranus";
			planetData.Mass = constants::uranus::mass;
			planetData.Radius = constants::uranus::radius;
			planetData.Velocity = 0;
			break;

		case Mercury:
			planetData.Name = "Mercury";
			planetData.Mass = constants::mercury::mass;
			planetData.Radius = constants::mercury::radius;
			planetData.Velocity = 0;
			break;

		case Pluto:
			planetData.Name = "Pluto";
			planetData.Mass = constants::pluto::mass;
			planetData.Radius = constants::pluto::radius;
			planetData.Velocity = 0;
			break;

		case Venus:
			planetData.Name = "Venus";
			planetData.Mass = constants::venus::mass;
			planetData.Radius = constants::venus::radius;
			planetData.Velocity = 0;
			break;

		case Neptune:
			planetData.Name = "Neptune";
			planetData.Mass = constants::neptune::mass;
			planetData.Radius = constants::neptune::radius;
			planetData.Velocity = 0;
			break;
		}

		sfTextContainer[line][0].setString(planetData.Name);
		
		std::ostringstream massString;
		massString << std::scientific << std::setprecision(2) << planetData.Mass << " kg?";
		sfTextContainer[line][1].setString(massString.str());

		std::ostringstream radiusString;
		radiusString << std::scientific << std::setprecision(2) << planetData.Radius << " m?";
		sfTextContainer[line][2].setString(radiusString.str());

		sfTextContainer[line][3].setString("tbd.");


	}

	void setString(std::string string, int line, int column) {
		sfTextContainer[line][column].setString(string);
	}

	void draw(sf::RenderWindow* renderWindow, bool drawBoundaries = false) {
		renderWindow->draw(backgroundVertexArray);
		renderWindow->draw(borderVertexArray);
		for (std::vector col : sfTextContainer) {
			for (sf::Text text : col) {
				renderWindow->draw(text);
			}
		}
		
	}

	void autoSetHeigth() {
		size.y = 2 * padding + lineNo * (fontSize + lineDistance);
		backgroundVertexArray = createRoundedRect(position, size.x, size.y, cornerRadius, windowBackgroundColor);
		borderVertexArray = createRoundedRectBorder(position, size.x, size.y, cornerRadius, borderLineThickness, windowBorderColor);
	}
};
