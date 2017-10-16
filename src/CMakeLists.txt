cmake_minimum_required(VERSION 2.8)
add_subdirectory(Core)
# add_subdirectory(Game)
add_subdirectory(Editor)

# add_library(Framework STATIC ${FRAMEWORK_SRCS} ${FRAMEWORK_HDRS})
set(CORE_HDRS ${CORE_HDRS} PARENT_SCOPE)
set(CORE_SRCS ${CORE_SRCS} PARENT_SCOPE)

# add_library(Game ${GAME_SRCS} ${GAME_HDRS})
# set(GAME_HDRS ${GAME_HDRS} PARENT_SCOPE)
# set(GAME_SRCS ${GAME_SRCS} PARENT_SCOPE)

# add_library(Editor STATIC ${EDITOR_SRCS} ${EDITOR_HDRS})
set(EDITOR_HDRS ${EDITOR_HDRS} PARENT_SCOPE)
set(EDITOR_SRCS ${EDITOR_SRCS} PARENT_SCOPE)
