cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

project(MiniYaml)

add_library(Yaml STATIC "Yaml.cpp" "include/MiniYaml/Yaml.hpp")
target_include_directories(Yaml PRIVATE "include/MiniYaml")
