cmake_minimum_required(VERSION 3.14)

project(CellularAutomata)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-3.0.0")
include_directories(${SFML_ROOT}/include)
link_directories(${SFML_ROOT}/lib)

add_executable(CellularAutomata
    src/main.cpp
)

target_link_libraries(CellularAutomata 
	sfml-graphics
	sfml-window
	sfml-system
)