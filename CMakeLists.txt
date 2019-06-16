cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mazemaze)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(Intl REQUIRED MODULE)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics system REQUIRED)
find_package(SFGUI QUIET MODULE)

if (NOT SFGUI_FOUND)
    set(SFGUI_BUILD_EXAMPLES OFF CACHE BOOL "Build examples." FORCE)

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/sfgui/CMakeLists.txt)
        message(STATUS "Found SFGUI submodule in " ${CMAKE_CURRENT_LIST_DIR}/sfgui)
        add_subdirectory(sfgui)
    else (EXISTS ${CMAKE_CURRENT_LIST_DIR}/sfgui/CMakeLists.txt)
        message(FATAL "SFGUI not found")
    endif (EXISTS ${CMAKE_CURRENT_LIST_DIR}/sfgui/CMakeLists.txt)
endif (NOT SFGUI_FOUND)

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
    src/FpsCalculator.cpp
    src/FpsCalculator.hpp
    src/utils.hpp
    src/utils.cpp
    src/Gui/Background.cpp
    src/Gui/Background.hpp
    src/Gui/MainMenu.cpp
    src/Gui/MainMenu.hpp
    src/Gui/Gui.cpp
    src/Gui/Gui.hpp
    src/Gui/State.cpp
    src/Gui/State.hpp
    src/Gui/States/Empty.cpp
    src/Gui/States/Empty.hpp
    src/Gui/States/Main.cpp
    src/Gui/States/Main.hpp
    src/Gui/States/Options.cpp
    src/Gui/States/Options.hpp
    src/Gui/States/Pause.cpp
    src/Gui/States/Pause.hpp
    src/Gui/States/Win.cpp
    src/Gui/States/Win.hpp
    src/Gui/States/NewGame.cpp
    src/Gui/States/NewGame.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${Intl_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SFGUI_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE lib)

target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARY})
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_NLS=1)

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

add_subdirectory(locale)
