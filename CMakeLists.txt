cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mazemaze)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

find_package(Intl REQUIRED MODULE)
find_package(Config++ REQUIRED MODULE)
find_package(OpenGL REQUIRED)
find_package(SFML COMPONENTS window graphics system REQUIRED)
find_package(SFGUI 0.4 QUIET MODULE)

if (NOT SFGUI_FOUND)
    if (EXISTS ${CMAKE_SOURCE_DIR}/sfgui/CMakeLists.txt)
        message(STATUS "Found SFGUI submodule in " ${CMAKE_SOURCE_DIR}/sfgui)
    else (EXISTS ${CMAKE_SOURCE_DIR}/sfgui/CMakeLists.txt)
        find_package(Git MODULE)

        if (GIT_FOUND)
            message(STATUS "Downloaded SFGUI submodule in " ${CMAKE_SOURCE_DIR}/sfgui)
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule init
                            COMMAND ${GIT_EXECUTABLE} submodule update
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/sfgui)
        else (GIT_FOUND)
            message(FATAL_ERROR "SFGUI not found and can't be downloaded")
        endif (GIT_FOUND)
    endif (EXISTS ${CMAKE_SOURCE_DIR}/sfgui/CMakeLists.txt)

    set(SFGUI_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared library." FORCE)
    set(SFGUI_BUILD_EXAMPLES    OFF CACHE BOOL "Build examples."       FORCE)

    add_subdirectory(sfgui)

    set(SFGUI_LIBRARY     SFGUI)
    set(SFGUI_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/sfgui/include/)
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
    src/Settings.cpp
    src/Settings.hpp
    src/Saver.cpp
    src/Saver.hpp
    src/FpsCalculator.cpp
    src/FpsCalculator.hpp
    src/utils.hpp
    src/utils.cpp
    src/Skybox.cpp
    src/Skybox.hpp
    src/Gui/Background.cpp
    src/Gui/Background.hpp
    src/Gui/MainMenu.cpp
    src/Gui/MainMenu.hpp
    src/Gui/Gui.cpp
    src/Gui/Gui.hpp
    src/Gui/State.cpp
    src/Gui/State.hpp
    src/Gui/States/Hud.cpp
    src/Gui/States/Hud.hpp
    src/Gui/States/Main.cpp
    src/Gui/States/Main.hpp
    src/Gui/States/OptionsMenu.cpp
    src/Gui/States/OptionsMenu.hpp
    src/Gui/States/Options.cpp
    src/Gui/States/Options.hpp
    src/Gui/States/OptionsGraphics.cpp
    src/Gui/States/OptionsGraphics.hpp
    src/Gui/States/OptionsOther.cpp
    src/Gui/States/OptionsOther.hpp
    src/Gui/States/OptionsControls.cpp
    src/Gui/States/OptionsControlsKeys.cpp
    src/Gui/States/OptionsControlsKeyChangeWindow.cpp
    src/Gui/States/OptionsControls.hpp
    src/Gui/States/Pause.cpp
    src/Gui/States/Pause.hpp
    src/Gui/States/Win.cpp
    src/Gui/States/Win.hpp
    src/Gui/States/NewGame.cpp
    src/Gui/States/NewGame.hpp
    src/MazeRenderers/Classic.cpp
    src/MazeRenderers/Classic.hpp
    src/MazeRenderers/Gray.cpp
    src/MazeRenderers/Gray.hpp
    src/MazeRenderers/Brick.cpp
    src/MazeRenderers/Brick.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${Intl_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CONFIG++_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${SFGUI_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE lib)

target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CONFIG++_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SFGUI_LIBRARY})
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_NLS=1)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt" CACHE PATH "Install prefix" FORCE)
endif (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

configure_file(mazemaze.desktop.in mazemaze.desktop)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_NAME})

install(DIRECTORY data
        DESTINATION ${PROJECT_NAME})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mazemaze.desktop
        DESTINATION ~/.local/share/applications
        COMPONENT DESKTOP_ENTRY)

add_subdirectory(locale)
