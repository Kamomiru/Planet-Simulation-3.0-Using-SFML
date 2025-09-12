
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "constants.h"
#include "vectorHelper.h"
#include <cmath>

#include <iostream>

class CelestialObject { //TODO: Implement Verlet integration
private:
    std::string name;
    sf::CircleShape shape;
    sf::Color color;

	//Simulation Relevant Attributes
    double mass;
    double radius;
    std::vector<double> position;
	std::vector<double> velocity; //TODO: is this needed for velet integration? Maybe store previous position instead of velocity?

    double startingKineticEnergy;
    
    sf::VertexArray trajectory;

    

public:
    //Constructor
    CelestialObject(std::string n, double m, double r, std::vector<double> pos, std::vector<double> vel, sf::Color col) {
        name = n;
        mass = m;
        radius = r;
        position = pos;
        velocity = vel;

        //Apply Attributes to sfml-object shape
        shape.setRadius(radius);
        shape.setOrigin({ static_cast<float>(radius),static_cast<float>(radius) });
        shape.setPosition(vectorToSfVector(position));
        shape.setFillColor(col);

        //Setup trajectory VertexArray
        trajectory.setPrimitiveType(sf::PrimitiveType::LineStrip);
        trajectory.append(vectorToVertex(position));

        //startingKineticEnergy = this->calcKineticEnergy();
    }

    //show object in RenderWindow
    void draw(bool drawTrajectory = false) {
		//TODO: Will this function be needed in current implementation?
    }

#pragma region Get Functions
    //Get CelestialObject name
    std::string getName() const {
        return name;
    }

    //Get CelestialObject mass
    double getMass() {
        return mass;
    }

    //Get CelestialObject Position
    std::vector<double> getPosition() {
        return position;
    }

    //Get CelestialObject Velocity
    std::vector<double> getVelocity() {
        return velocity;
    }

    double getStartingKineticEnergy() {
        return startingKineticEnergy;
    }

    sf::Drawable& getDrawable() {
        return shape;
	}
#pragma endregion

    //Overload == Operator
    bool operator==(const CelestialObject& otherCelestialObject) const {
        return (name == otherCelestialObject.getName());
    }
};

class Simulation {
public:
    int steps;
    double startingSystemEnergy;
    std::vector<CelestialObject> CelestialObjectContainer;
    std::vector<pairContainer<CelestialObject>> CelestialObjectPairContainer;

    //Constructor
    Simulation(std::vector<CelestialObject> CelestialObjectContainer) {
        CelestialObjectContainer = CelestialObjectContainer;
        CelestialObjectPairContainer = getPairs(CelestialObjectContainer);
        startingSystemEnergy = calcTotalKineticEnergy() - calcTotalPotentialEnergy();
        steps = 0;
    }

#pragma region EnergyFunctions
    //Calculate total Kinetic Energy of all CelestialObjects/System
    double calcTotalKineticEnergy() {
		//TODO: Implement Kinetic Energy Calculation for Verlet integration
    }

    //Calculate Potential Energy between two CelestialObjects
    double calcPotentialEnergy(pairContainer<CelestialObject> CelestialObjectPair) {
		//TODO: Implement Potential Energy Calculation for Verlet integration
    }

    //Calculate Total Potential Energy of CelestialObjectary System
    double calcTotalPotentialEnergy() {
		//TODO: Implement Total Potential Energy Calculation for Verlet integration
    }

    //Calculate Relative Energy Increase relative to starting System Energy. Return value is in %
    double calcRelativeEnergyIncrease() {
		//TODO: Check functionality after implementing energy calculations for Verlet integration
        double percentage{ std::abs(calcSystemEnergy() / startingSystemEnergy - 1.0f) * 100 };
        return std::round(percentage * 100.0) / 100.0;
    }

    //Calculate Current System Energy
    double calcSystemEnergy() {
        return calcTotalKineticEnergy() - calcTotalPotentialEnergy();
    }
#pragma endregion

    


};

//TODO: Assert if still needed?
//initializes Simulation.
//Function Body also holds CelestialObject starting Attributes!!!
inline Simulation initSimulation() { //has to be below classes, so the function knows how to handle CelestialObject objects!

    //Add CelestialObjects here
    CelestialObject earth("earth", constants::earth::mass, constants::earth::radius, { 300.0f,600.0f }, { 10.0,-2.0 }, sf::Color::Blue);
    CelestialObject moon("moon", constants::moon::mass, constants::moon::radius, { 350.0f, 800.0f }, { -4.0,-2.0 }, sf::Color::White);
    CelestialObject moon2("moon2", constants::moon::mass, constants::moon::radius, { 650.0f, 700.0f }, { 19.0,5.0 }, sf::Color::Red);
    CelestialObject moon3("moon3", constants::moon::mass, constants::moon::radius, { 100.0f, 100.0f }, { 16.0f, -3.8f }, sf::Color::Green);
    CelestialObject moon4("moon4", constants::moon::mass, constants::moon::radius, { 350.0f, 900.0f }, { 18.0,-5.0 }, sf::Color::Blue);

    //Append them to the CelestialObjectContainer
    std::vector<CelestialObject> CelestialObjectContainer{ earth, moon, moon2, moon3, moon4 };

    //Call simulation constructor
    Simulation simulation(CelestialObjectContainer);

    return simulation;
}

