#pragma once

namespace constants {
	//Scale factor
	constexpr double scale{ 10e-6 }; //With factor 10e-6, 1000000m --> 1 unit in simulation = 1px

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

}