#include "vectorHelper.h"


double vectorLength(std::vector<double> vector) {
	return std::sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
}

//calculate vector angle in Radiants for 2D vectors. Should always output a Value between [ 0 ; 2*pi ]
double vectorAngle(std::vector<double> vector) {

	bool IsZeroVector{ false }; //check for valid vector input
	if (vector[0] == 0 && vector[1] == 0) {
		IsZeroVector = true;
		std::cout << IsZeroVector;
	}
	assert((!IsZeroVector) && "The given Vector is the zero Vector, hence an angle cannot be defined!");

	double result{ atan(vector[1] / vector[0]) }; //actual calculation

	if (vector[0] >= 0) { //Quadrants 1 and 4

		if ((result < 10e-6) && (result > -10e-6)) { //Implementing cutoff Value so function returns 0 for small angles
			return 0;
			std::cout << "0-Value Cutoff!\n";
		}
		else {
			if (vector[1] < 0) {
				return result + 2 * pi;
			}
			else {
				return result;
			}
		}
	}
	else if (vector[0] < 0) { //Quadrants 2 and 3
		if ((result < 10e-6) && (result > -10e-6)) { //Implementing cutoff Value so function returns 0 for small angles
			return 0 + pi;
			std::cout << "0-Value Cutoff!\n";
		}
		else {
			return result + pi;
		}

	}
}

//helper function to convert 2D std::vector<double> to sf::Vertex
sf::Vertex vectorToVertex(std::vector<double> vector) {
	sf::Vertex Vertex{ vectorToSfVector(vector) };
	return Vertex;
}

sf::Vector2f vectorToSfVector(std::vector<double> vector) {
	return sf::Vector2f{ static_cast<float>(vector[0]),static_cast<float>(vector[1]) };
}

//overload + operator to support elementwise addition via + symbol (very cool)
std::vector<double>& operator+(const std::vector<double>& a, const std::vector<double>& b) { //vectors a and b are being passed by reference due to the & sign
	if (a.size() != b.size()) { //Check if vectors a and b have the same dimension
		throw std::invalid_argument("Vectors must have the same size");
	}
	std::vector<double> result(a.size());
	for (int i{ 0 }; i < a.size(); i++) {
		result[i] = a[i] + b[i]; //element wise addition
	}
	return result;
}

//overload - operator to support elementwise addition via - symbol (very cool)
std::vector<double>& operator-(const std::vector<double>& a, const std::vector<double>& b) { //vectors a and b are being passed by reference due to the & sign
	if (a.size() != b.size()) { //Check if vectors a and b have the same dimension
		throw std::invalid_argument("Vectors must have the same size");
	}
	std::vector<double> result(a.size());
	for (int i{ 0 }; i < a.size(); i++) {
		result[i] = a[i] - b[i]; //element wise addition
	}
	return result;
}

//overload += operator to support elementwise addition via += symbol
std::vector<double>& operator+=(std::vector<double>& a, const std::vector<double>& b) { //vectors a and b are being passed by reference due to the & sign.  Function also returns a reference type
	if (a.size() != b.size()) { //Check if vectors a and b have the same dimension
		throw std::invalid_argument("Vectors must have the same size");
	}
	for (int i{ 0 }; i < a.size(); ++i) {
		a[i] += b[i];
	}
	return a;
}