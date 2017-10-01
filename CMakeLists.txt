cmake_minimum_required(VERSION 2.8)

project(Galaxian1979)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# set(BUILD_EDITOR TRUE CACHE BOOL "Enable building of editor.")
# set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(extern)
add_subdirectory(src)

include_directories(
  src
  ${DEPENDENCIES_INCLUDE}
)

add_library(Framework ${FRAMEWORK_SRCS} ${FRAMEWORK_HDRS})
add_library(Game ${GAME_SRCS} ${GAME_HDRS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${PROJECT_NAME} src/Game/main.cpp)
  target_link_libraries(${PROJECT_NAME}
    Framework
    Game
    ${DEPENDENCIES_LIBS}
  )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_library(Editor ${EDITOR_SRCS} ${EDITOR_HDRS})
  add_executable(${PROJECT_NAME} src/Editor/main.cpp)
  target_link_libraries(${PROJECT_NAME}
    Framework
    Editor
    Game
    ${DEPENDENCIES_LIBS}
  )
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
