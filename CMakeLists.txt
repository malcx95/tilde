cmake_minimum_required(VERSION 3.0)
project(tilde)

set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wno-unused -pedantic -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB OhCrapImBlind_SRC
    "*.cpp"
    "*.hpp"
    )


set(EXECUTABLE_NAME "tilde")


add_executable(${EXECUTABLE_NAME} ${OhCrapImBlind_SRC})

# Detect and add SFML
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(${SFML_FOUND})
    # message("${SFML_INCLUDE_DIR}")
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

