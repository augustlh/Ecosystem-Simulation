cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

project(Ecosystem-Simulation)

add_subdirectory("external/SDL")
add_subdirectory("external/MiniYaml")

set(LIB_SOURCES
    "src/Camera.cpp"
    "src/Generation.cpp"
    "src/Map.cpp"
    "src/Renderer.cpp"
    "src/Simulation.cpp"
    "src/Window.cpp"
)

set(HEADERS
    "include/Camera.h"
    "include/Ecosim.h"
    "include/Generation.h"
    "include/Interfaces.h"
    "include/Map.h"
    "include/Renderer.h"
    "include/Simulation.h"
    "include/Statistics.h"
    "include/Vector2.h"
    "include/Window.h"
)

add_library(EcosimLib SHARED ${LIB_SOURCES} ${HEADERS})

target_include_directories(EcosimLib PRIVATE "include")
target_include_directories(EcosimLib PRIVATE "external/SDL/include")
target_include_directories(EcosimLib PRIVATE "external/MiniYaml/include")
target_include_directories(EcosimLib PRIVATE "external/PerlinNoise/include")
target_link_libraries(EcosimLib SDL3-shared Yaml)
target_compile_definitions(EcosimLib PRIVATE "ECOSIM_EXPORT")

add_executable(Ecosim "src/Main.cpp")

target_include_directories(Ecosim PRIVATE "include")
target_link_libraries(Ecosim EcosimLib)

add_custom_target(run COMMAND Ecosim DEPENDS Ecosim WORKING_DIRECTORY ${CMAKE_PROJECT_DIR})

# Unit testing

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
add_executable(${TEST_NAME} ${TEST_SOURCE})
target_link_libraries(${TEST_NAME} EcosimLib SDL3-shared Yaml)
target_include_directories(${TEST_NAME} PRIVATE "tests")
    target_include_directories(${TEST_NAME} PRIVATE "include")
    target_include_directories(${TEST_NAME} PRIVATE "external/SDL/include")
    target_include_directories(${TEST_NAME} PRIVATE "external/MiniYaml/include")
    set_target_properties(${TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
endforeach()
