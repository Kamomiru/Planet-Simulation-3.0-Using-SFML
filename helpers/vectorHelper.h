#pragma once
#include <vector>
#include <stdexcept>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

constexpr double pi = 3.14159265358979323846;

//pair container that holds arbitrary types
template <typename T>
struct  pairContainer {
	T a;
	T b;
};

double vectorLength(std::vector<double> vector);
double vectorAngle(std::vector<double> vector);

sf::Vertex vectorToVertex(std::vector<double> vector);
sf::Vector2f vectorToSfVector(std::vector<double> vector);

std::vector<double> operator+(const std::vector<double>& a, const std::vector<double>& b);
std::vector<double> operator*(const std::vector<double>& v, const double s);
std::vector<double> operator*(const double s, const std::vector<double>& v);
std::vector<double> operator-(const std::vector<double>& a, const std::vector<double>& b);

std::vector<double>& operator+=(std::vector<double>& a, const std::vector<double>& b);

bool isElementOfVector(std::string s, std::vector<std::string> vec);

//creates all possible pair combination of objects in a vector
template <typename T> //Sets function up so that the output is a vector that holds Object pairs of a certain type and gets a vector with Objects of that type.
std::vector<pairContainer<T*>> getPairs(std::vector<T*> inputVector) {
	std::vector<pairContainer<T*>> result; //Setup output vector

	for (int i{ 0 }; i < inputVector.size(); i++) { //For each element in inputVector
		for (int j{ i + 1 }; j < inputVector.size(); j++) {
			result.push_back(pairContainer<T*> {inputVector[i], inputVector[j]});
		}
	}
	return result;
}

//creates all possible pair combination of objects in a vector
template <typename T>
std::vector<pairContainer<T>> getPairs(std::vector<T> inputVector) {
	std::vector<pairContainer<T>> result;

	for (int i{ 0 }; i < inputVector.size(); i++) {
		for (int j{ i + 1 }; j < inputVector.size(); j++) {
			result.push_back(pairContainer<T> {inputVector[i], inputVector[j]});
		}
	}
	return result;
}

//helper function to strip vector of given planet element
template <typename T>
std::vector<T> stripVector(const std::vector<T>& vector, const T& value) { //using pass by reference here is optional!!!! it says nothing about what kind of type the vector getting passed in has to hold!!!
	std::vector<T> strippedVector;
	for (const T& element : vector) { //for each element of type T in given vector do...
		if (!(element == value)) {
			strippedVector.push_back(element);
		}
	}
	return strippedVector;
}
