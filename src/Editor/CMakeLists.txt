cmake_minimum_required(VERSION 2.8)
set(EDITOR_HDRS
  "${CMAKE_CURRENT_SOURCE_DIR}/GalaxianEditor.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui_style.h"
  PARENT_SCOPE
)

set(EDITOR_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/GalaxianEditor.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Systems.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl.cpp"
  PARENT_SCOPE
)
