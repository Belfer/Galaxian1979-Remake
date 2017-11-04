cmake_minimum_required(VERSION 2.8)

project(Galaxian1979)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(extern)
add_subdirectory(src)

include_directories(src ${DEPENDENCIES_INCLUDE})

add_library(Core ${CORE_SRCS} ${CORE_HDRS})
target_link_libraries(Core ${DEPENDENCIES_LIBS})

add_library(Graphics ${GRAPHICS_SRCS} ${GRAPHICS_HDRS})
target_link_libraries(Graphics ${DEPENDENCIES_LIBS})

add_library(Audio ${AUDIO_SRCS} ${AUDIO_HDRS})
target_link_libraries(Audio ${DEPENDENCIES_LIBS})

add_library(Game ${GAME_SRCS} ${GAME_HDRS})
target_link_libraries(Game Core Graphics Audio)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_executable(${PROJECT_NAME} src/Game/main.cpp)
  target_link_libraries(${PROJECT_NAME} Game)

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_library(Editor ${EDITOR_SRCS} ${EDITOR_HDRS})
  target_link_libraries(Editor Game Core Graphics Audio)

  add_executable(${PROJECT_NAME} src/Editor/main.cpp)
  target_link_libraries(${PROJECT_NAME} Editor)

endif(CMAKE_BUILD_TYPE STREQUAL "Release")
