# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Chomp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
        include
)

# Add the executable
add_executable(Main 
        src/board.cpp
        src/iterator.cpp
        src/evidence.cpp
        src/pattern.cpp
        main.cpp
)