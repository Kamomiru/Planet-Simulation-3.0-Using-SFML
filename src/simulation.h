
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "constants.h"
#include "../helpers/vectorHelper.h"
#include <cmath>

#include <iostream>

class CelestialObject { //TODO: Implement Verlet integration
private:
	std::string name = "default";
    sf::CircleShape shape;
	sf::Color color = sf::Color::White;

	//Simulation Relevant Attributes
    double mass = -1;
    double radius = -1;
    std::vector<double> position;
	std::vector<double> velocity; //TODO: is this needed for velet integration? Maybe store previous position instead of velocity?

    double startingKineticEnergy = -1;
    
    sf::VertexArray trajectory;

	bool objectSetupComplete = false;

public:
	//Default Constructor
    CelestialObject() {
        trajectory.setPrimitiveType(sf::PrimitiveType::LineStrip);
        trajectory.append(vectorToVertex(position));
    }

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

        startingKineticEnergy = this->calcStartingKineticEnergy();

		objectSetupComplete = true;
    }

    struct Builder {
		static void setName(CelestialObject& obj, const std::string& n) { 
            assert(obj.name == "default" && "Error: Name is already defined!");
            obj.name = n; }

        static void setColor(CelestialObject& obj, sf::Color c) { 
            assert(obj.color == sf::Color::White && "Error: Color is already defined!");
            obj.color = c; 
            obj.shape.setFillColor(c);
		}
        
        static void setMass(CelestialObject& obj, double m) {
            assert(obj.mass == -1 && "Error: Mass has alrady been set!");
			obj.mass = m;
		}
        
        static void setRadius(CelestialObject& obj, double r) {
            assert(obj.radius == -1 && "Error: Radius has alrady been set!");
            obj.radius = r;
            obj.shape.setRadius(obj.radius);
			obj.shape.setOrigin({ static_cast<float>(r),static_cast<float>(r) });
        }

        static void setPosition(CelestialObject& obj, std::vector<double> pos) {
            assert(obj.position.empty() && "Error: Position has alrady been set!");
			obj.position = pos;
            obj.shape.setPosition(vectorToSfVector(obj.position));
        }

        static void setVelocity(CelestialObject& obj, std::vector<double> vel) {
			assert(obj.velocity.empty() && "Error: Velocity has alrady been set!");
            obj.velocity = vel;
        }
        
        static void finalizeSetup(CelestialObject& obj) {
			assert(!obj.objectSetupComplete && "Error: Object setup is already complete!");
            assert(obj.name != "default" && "Error: Name was not set!");
            assert(obj.mass != -1 && "Error: Mass was not set!");
            assert(obj.radius != -1 && "Error: Radius was not set!");
            assert(!obj.position.empty() && "Error: Position was not set!");
            assert(!obj.velocity.empty() && "Error: Velocity was not set!");
            //Setup trajectory VertexArray
            obj.trajectory.append(vectorToVertex(obj.position));
			obj.startingKineticEnergy = obj.calcStartingKineticEnergy();
			
            obj.objectSetupComplete = true;
        }

    };

    double calcStartingKineticEnergy() {
		assert(startingKineticEnergy == -1 && "Error: StartingKineticEnergy has already been calculated!"); //Make sure this function is only called once!
		startingKineticEnergy = 0.5l * mass * (velocity[0] * velocity[0] + velocity[1] * velocity[1]);
		return startingKineticEnergy;
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
		return shape; //TODO: Implement to return both shape and trajectory
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
    double startingSystemEnergy = !1;
    std::vector<CelestialObject> celestialObjectContainer;
    std::vector<pairContainer<CelestialObject>> celestialObjectPairContainer;

    //Constructor
    Simulation(std::vector<CelestialObject> CelestialObjectContainer) {
        celestialObjectContainer = CelestialObjectContainer;
        celestialObjectPairContainer = getPairs(CelestialObjectContainer);
        //TODO: Implement Energy calculation for Simulation analysis. startingSystemEnergy = calcTotalKineticEnergy() - calcTotalPotentialEnergy();
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

