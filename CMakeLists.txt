cmake_minimum_required(VERSION 3.28)
project(Planet-Simulation-3.0-Using-SFML LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main
src/main.cpp
src/programWindow.h
src/simulation.h
src/simulationOLD.h
src/constants.h
src/programConfig.h
modes/baseMode.h
modes/DisplayTestMode.h
modes/modeHandler.h
modes/SimulationSetupMode.h
modes/SimulationTestMode.h
modes/StartupMode.h
helpers/vectorHelper.cpp
helpers/vectorHelper.h
helpers/customVertexArrays.cpp
helpers/customVertexArrays.h
sfml-entities/customSFMLEntities.h
)

target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
