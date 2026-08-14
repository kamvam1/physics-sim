cmake_minimum_required(VERSION 3.16)
project(PhysicsSim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: build type defaults to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Find SFML (installed via apt: libsfml-dev)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# --- Source files ---
# List every .cpp file that should be compiled into the executable.
set(SOURCES
    main.cpp
    Vector2D.cpp
)

add_executable(PhysicsSim ${SOURCES})

# Headers 
target_sources(PhysicsSim PRIVATE
    Vector2D.hpp
)

target_link_libraries(PhysicsSim sfml-graphics sfml-window sfml-system)

# Optional: put the binary in a predictable spot
set_target_properties(PhysicsSim PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})