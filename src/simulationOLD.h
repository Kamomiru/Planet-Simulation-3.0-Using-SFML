
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

	//Probably not working/usefull anymore with Verlet integration
    ////Calculate Kinetic Energy
    //double calcKineticEnergy() {
    //    return (0.5f) * mass * (velocity[0] * velocity[0] + velocity[1] * velocity[1]);
    //}

	//TODO: The following fuctions should be moved over to the simulation class!!

    //Calculate Distance Vector from one CelestialObject to another. distVector is relative to the CelestialObject!
    std::vector<double> calcDistVector(CelestialObject otherCelestialObject) {
        std::vector<double> distVector{ 0,0 };
        for (int i{ 0 }; i <= 1; i++) {
            distVector[i] = otherCelestialObject.getPosition()[i] - position[i];
        }
        return distVector;
    }

    //Calculate Acceleration due to other CelestialObject
    std::vector<double> calcAccel(CelestialObject otherCelestialObject) {
        std::vector<double> accelVector{ 0.0l, 0.0l };
        double celestialObjectDistance{ vectorLength(calcDistVector(otherCelestialObject)) }; //Distance to other CelestialObject
        double accelValue{ constants::gravConstant * otherCelestialObject.getMass() / (celestialObjectDistance * celestialObjectDistance) };  //acceleration of our CelestialObject due to anotherCelestialObject

        //convert acceleration-value to acceleration vector
        double angle{ vectorAngle(calcDistVector(otherCelestialObject)) };
        double cosValue{ std::cos(angle) };
        double sinValue{ std::sin(angle) };
        if (cosValue < 10e-12 && cosValue > -10e-12) {
            cosValue = 0;
        }
        if (sinValue < 10e-12 && sinValue > -10e-12) {
            sinValue = 0;
        }

        accelVector[0] = cosValue * accelValue;
        accelVector[1] = sinValue * accelValue;

        return accelVector;
    }

    //Calculate Acceleration due to all other CelestialObjects
    std::vector<double> calcTotalAccel(std::vector<CelestialObject*>& CelestialObjectContainer) { //pass by reference to a vector holding Pointers to CelestialObject Objects!
        std::vector<double> totalAccel{ 0.0f, 0.0f };
        std::vector<CelestialObject*> otherCelestialObjects{ stripVector(CelestialObjectContainer, this) }; //Create a vector containing all other CelestialObjects
        //this is a pointer, we dont want to dereference it using *this becuase otherCelestialObjects is a vector holding pointers. if we would do this we would get an error

        for (CelestialObject* CelestialObject : otherCelestialObjects) { //we have to iterate over CelestialObject* here bc the Vector holds pointers
            totalAccel += calcAccel(*CelestialObject);  //and then here we finally have to dereference so we can pass a CelestialObject object into our calcAccel function and not an Memory Adress
        }
        return totalAccel;
    }

    //Calculate Acceleration due to all other CelestialObjects
    std::vector<double> calcTotalAccel(std::vector<CelestialObject>& CelestialObjectContainer) {
        std::vector<double> totalAccel{ 0.0f, 0.0f };
        std::vector<CelestialObject> otherCelestialObjects{ stripVector(CelestialObjectContainer, *this) };

        for (CelestialObject CelestialObject : otherCelestialObjects) {
            totalAccel += calcAccel(CelestialObject);
        }
        return totalAccel;
    }

    //Update CelestialObjects velocity. Explicit euler Method
    void updateVelocity(std::vector<double> acceleration) {
        for (int i = 0; i <= 1; i++) {
            velocity[i] += acceleration[i] * constants::dt;
        }
    }

    //Update CelestialObject position
    void updatePosition() {
        for (int i = 0; i <= 1; i++) {
            position[i] += velocity[i] * constants::dt; //TODO: Overload * function to make math cleaner
        }
        shape.setPosition(vectorToSfVector(position));
        trajectory.append(vectorToVertex(position));
    }

    //Updates all Kinematic Attributes of a CelestialObject
    void updateAll(std::vector<CelestialObject*> CelestialObjectContainer) {
        this->updateVelocity(this->calcTotalAccel(CelestialObjectContainer));
        this->updatePosition();
    }

    //Updates all Kinematic Attributes of a CelestialObject
    void updateAll(std::vector<CelestialObject>& CelestialObjectContainer) {
        this->updateVelocity(this->calcTotalAccel(CelestialObjectContainer));
        this->updatePosition();
    }

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
        double totalKineticEnergy{ 0.0 };
        for (CelestialObject element : CelestialObjectContainer) {
            totalKineticEnergy += element.calcKineticEnergy();
        }
        return totalKineticEnergy;
    }

    //Calculate Potential Energy between two CelestialObjects
    double calcPotentialEnergy(pairContainer<CelestialObject> CelestialObjectPair) {
        double distance = vectorLength(CelestialObjectPair.a.calcDistVector(CelestialObjectPair.b));
        return constants::gravConstant * (CelestialObjectPair.a.getMass() * CelestialObjectPair.b.getMass()) / distance;
    }

    //Calculate Total Potential Energy of CelestialObjectary System
    double calcTotalPotentialEnergy() {
        double result{ 0 };
        for (int i{ 0 }; i < CelestialObjectPairContainer.size(); i++) {
            result += calcPotentialEnergy(CelestialObjectPairContainer[i]);
        }
        return result;
    }

    //Calculate Relative Energy Increase relative to starting System Energy. Return value is in %
    double calcRelativeEnergyIncrease() {
        double percentage{ std::abs(calcSystemEnergy() / startingSystemEnergy - 1.0f) * 100 };
        return std::round(percentage * 100.0) / 100.0;
    }

    //Calculate Current System Energy
    double calcSystemEnergy() {
        return calcTotalKineticEnergy() - calcTotalPotentialEnergy();
    }
#pragma endregion

    //increment Simulation to next timestep
    void updateAllAttributes() {
        for (CelestialObject& CelestialObject : CelestialObjectContainer) { //We NEED to use CelestialObject& here to pass by reference. otherwise a copy of the object will be passed and hence we dont change our objects Attributes!!!
            CelestialObject.updateAll(CelestialObjectContainer);
        }
        ++steps;
    }

    //Draws all CelestialObjects in globally defined Window
    void drawAll(bool drawTrajectory = false) {
        window.setView(view);
        for (CelestialObject& CelestialObject : CelestialObjectContainer) {
            CelestialObject.draw(drawTrajectory);
        }
    }


};


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

