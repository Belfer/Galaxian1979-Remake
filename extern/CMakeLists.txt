# Find packages.
find_package(OpenGL REQUIRED) 

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

set(ENTITYX_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ENTITYX_BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(ENTITYX_RUN_BENCHMARKS OFF CACHE BOOL "" FORCE)
add_subdirectory(entityx)

set(BuildTests OFF CACHE BOOL "" FORCE)
add_subdirectory(json)

add_subdirectory(glad)
add_subdirectory(soil)
add_subdirectory(imgui)

set(DEPENDENCIES_INCLUDE
  ${CMAKE_CURRENT_SOURCE_DIR}/glad/include
  ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/soil/src
  ${CMAKE_CURRENT_SOURCE_DIR}/json/src
  ${CMAKE_CURRENT_SOURCE_DIR}/entityx
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  PARENT_SCOPE
)

set(DEPENDENCIES_LIBS
  ${OPENGL_LIBRARY}
  glad
  glfw
  SOIL
  entityx
  imgui
  PARENT_SCOPE
)
