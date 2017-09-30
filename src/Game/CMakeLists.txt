cmake_minimum_required(VERSION 2.8)
set(GAME_HDRS
  "${CMAKE_CURRENT_SOURCE_DIR}/Components.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Events.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Factory.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/GalaxianApp.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems.hpp"
  PARENT_SCOPE
)

set(GAME_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/Factory.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/GalaxianApp.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems.cpp"
  PARENT_SCOPE
)
