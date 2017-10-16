cmake_minimum_required(VERSION 2.8)

project(Galaxian1979)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# add_definitions(-DPROJECT_DIR=${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
add_subdirectory(extern)
add_subdirectory(src)

include_directories(src ${DEPENDENCIES_INCLUDE})

add_library(Core ${CORE_SRCS} ${CORE_HDRS})
target_link_libraries(Core ${DEPENDENCIES_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  # add_library(Game ${GAME_SRCS} ${GAME_HDRS})
  # target_link_libraries(Game Core)

  add_executable(${PROJECT_NAME} src/Game/main.cpp)
  target_link_libraries(${PROJECT_NAME} Game)

elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_library(Editor ${EDITOR_SRCS} ${EDITOR_HDRS})
  target_link_libraries(Editor Core)

  add_executable(${PROJECT_NAME} src/Editor/main.cpp)
  target_link_libraries(${PROJECT_NAME} Editor)

endif(CMAKE_BUILD_TYPE STREQUAL "Release")
