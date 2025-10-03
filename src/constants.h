 #pragma once

namespace constants {
	//Scale factor
	constexpr double scale{ 1e-6 }; //With factor 1e-6, 1.000.000m --> 1 unit in simulation = 1px

	constexpr double gravConstant{ 6.6743e-11 * scale }; //Gravitational Constant in m³/kg/s²

	constexpr double dt{ 0.1 };

	namespace earth {
		constexpr double radius{ 6371 * 1000 * scale }; //Earth radius in meters
		constexpr double mass{ 5.9722e24 * scale }; //Earth mass in kg
	}

	namespace moon {
		constexpr double radius{ 1737.4 * 1000 * scale }; //Moon radius in meters
		constexpr double mass{ 0.07346e24 * scale }; //Moon mass in kg
	}

	namespace mars {
		constexpr double radius{ 3390 * 1000 * scale }; //Mars radius in meters
		constexpr double mass{ 6.42e23 * scale }; //Moon mass in kg
	}

	namespace jupiter {
		constexpr double radius{ 69911 * 1000 * scale };  // Jupiter radius in meters
		constexpr double mass{ 1.898e27 * scale };        // Jupiter mass in kg
	}

	namespace saturn {
		constexpr double radius{ 58232 * 1000 * scale };  // Saturn radius in meters
		constexpr double mass{ 5.683e26 * scale };        // Saturn mass in kg
	}

	namespace uranus {
		constexpr double radius{ 25362 * 1000 * scale };  // Uranus radius in meters
		constexpr double mass{ 8.681e25 * scale };        // Uranus mass in kg
	}

	namespace mercury {
		constexpr double radius{ 2439.7 * 1000 * scale }; // Mercury radius in meters
		constexpr double mass{ 3.3011e23 * scale };       // Mercury mass in kg
	}
	
	namespace pluto {
		constexpr double radius{ 1187 * 1000 * scale };
		constexpr double mass{ 1.303e22 * scale };
	}

	namespace venus {
		constexpr double radius{ 6051.8 * 1000 * scale }; // Venus radius in meters
		constexpr double mass{ 4.867e24 * scale };        // Venus mass in kg
	}

	namespace neptune {
		constexpr double radius{ 24622 * 1000 * scale }; // Neptune radius in meters
		constexpr double mass{ 1.024e26 * scale };       // Neptune mass in kg
	}

}