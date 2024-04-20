cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

project(Ecosystem-Simulation)

add_subdirectory("external/SDL")
add_subdirectory("external/MiniYaml")

set(SOURCES
    "src/Camera.cpp"
    "src/Main.cpp"
    "src/Map.cpp"
    "src/Renderer.cpp"
    "src/Simulation.cpp"
    "src/Window.cpp"
)

set(HEADERS
    "include/Camera.h"
    "include/Exceptions.h"
    "include/Interfaces.h"
    "include/Map.h"
    "include/Renderer.h"
    "include/Simulation.h"
    "include/Statistics.h"
    "include/Vector2.h"
    "include/Window.h"
)

add_executable(Ecosim ${SOURCES} ${HEADERS})
target_link_libraries(Ecosim SDL3-shared Yaml)

target_include_directories(Ecosim PRIVATE "include")
target_include_directories(Ecosim PRIVATE "external/SDL/include")
target_include_directories(Ecosim PRIVATE "external/MiniYaml/include")
target_include_directories(Ecosim PRIVATE "external/PerlinNoise/include")

add_custom_target(run COMMAND Ecosim DEPENDS Ecosim WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})