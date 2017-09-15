cmake_minimum_required(VERSION 2.8)

project(Galaxian1979)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# Header files
file(GLOB_RECURSE FRAMEWORK_LIB_HDRS
    src/Framework/*.inl
    src/Framework/*.hpp
    src/Framework/*.h
)

file(GLOB_RECURSE GAME_HDRS
    src/Game/*.inl
    src/Game/*.hpp
    src/Game/*.h
)

# Source files
file(GLOB_RECURSE FRAMEWORK_LIB_SRCS
    src/Framework/*.cpp
    src/Framework/*.cc
    src/Framework/*.c
)

file(GLOB_RECURSE GAME_SRCS
    src/Game/*.cpp
    src/Game/*.cc
    src/Game/*.c
)

add_library(NHTVFramework ${FRAMEWORK_LIB_HDRS} ${FRAMEWORK_LIB_SRCS})

# Find packages.
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

# Define external modules cmake path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
find_package(SOIL REQUIRED)
find_package(TinyXML REQUIRED)
find_package(EntityX REQUIRED)

include_directories(
    src/Framework
    src/Game
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${SOIL_INCLUDE_DIRS}
    ${TINYXML_INCLUDE_DIR}
    ${ENTITYX_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${GAME_HDRS} ${GAME_SRCS})

target_link_libraries(${PROJECT_NAME}
    NHTVFramework
    ${OPENGL_LIBRARY}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${SOIL_LIBRARIES}
    ${TINYXML_LIBRARIES}
    ${ENTITYX_LIBRARY}
)
