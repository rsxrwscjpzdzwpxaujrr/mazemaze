cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mazemaze)

find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics system REQUIRED)
find_package(SFGUI QUIET)

if (SFGUI_FOUND)
    message(STATUS "Found SFGUI in " ${SFGUI_DIR})
else (SFGUI_FOUND)
    set(SFGUI_BUILD_EXAMPLES OFF CACHE BOOL "Build examples." FORCE)

    add_subdirectory(sfgui)
endif (SFGUI_FOUND)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_sources(
    ${PROJECT_NAME}
    PRIVATE
    src/Camera.cpp
    src/Camera.hpp
    src/Chunk.cpp
    src/Chunk.hpp
    src/Game.cpp
    src/Game.hpp
    src/GraphicEngine.cpp
    src/GraphicEngine.hpp
    src/IRenderable.cpp
    src/IRenderable.hpp
    src/ITickable.cpp
    src/ITickable.hpp
    src/main.cpp
    src/MainMenu.cpp
    src/MainMenu.hpp
    src/MenuState.cpp
    src/MenuState.hpp
    src/Maze.cpp
    src/Maze.hpp
    src/MazeRenderer.cpp
    src/MazeRenderer.hpp
    src/Player.cpp
    src/Player.hpp
    src/StarSky.cpp
    src/StarSky.hpp
    src/Saver.cpp
    src/Saver.hpp
    src/Loader.cpp
    src/Loader.hpp
    src/FpsCalculator.cpp
    src/FpsCalculator.hpp
    src/MenuStates/Empty.cpp
    src/MenuStates/Empty.hpp
    src/MenuStates/Main.cpp
    src/MenuStates/Main.hpp
    src/MenuStates/Options.cpp
    src/MenuStates/Options.hpp
    src/MenuStates/Pause.cpp
    src/MenuStates/Pause.hpp
    src/MenuStates/Win.cpp
    src/MenuStates/Win.hpp
    src/MenuStates/NewGame.cpp
    src/MenuStates/NewGame.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${SFGUI_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} SFGUI)
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-unused-macros
        -Wno-exit-time-destructors
        -Wno-global-constructors
        -Wno-gnu-zero-variadic-macro-arguments
        -Wno-documentation
        -Wno-shadow
        -Wno-padded
        -Wno-conversion
        -Wno-switch-enum)
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
