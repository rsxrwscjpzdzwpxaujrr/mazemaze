cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)
set(OpenGL_GL_PREFERENCE LEGACY)

project(mazemaze)

find_package(Intl REQUIRED MODULE)
find_package(jsoncpp REQUIRED CONFIG)
find_package(OpenGL REQUIRED)
find_package(SFGUI 0.4 QUIET CONFIG NO_CMAKE_PACKAGE_REGISTRY)
find_package(Threads REQUIRED)

set(SOURCES
    src/Camera.cpp
    src/Chunk.cpp
    src/Game.cpp
    src/GraphicEngine.cpp
    src/IRenderable.cpp
    src/ITickable.cpp
    src/main.cpp
    src/Maze.cpp
    src/MazeRenderer.cpp
    src/Player.cpp
    src/StarSky.cpp
    src/Settings.cpp
    src/Saver.cpp
    src/FpsCalculator.cpp
    src/utils.cpp
    src/Skybox.cpp
    src/Logger.cpp
    src/Gui/Background.cpp
    src/Gui/MainMenu.cpp
    src/Gui/Gui.cpp
    src/Gui/State.cpp
    src/Gui/States/Hud.cpp
    src/Gui/States/Main.cpp
    src/Gui/States/OptionsMenu.cpp
    src/Gui/States/Options.cpp
    src/Gui/States/OptionsGraphics.cpp
    src/Gui/States/OptionsOther.cpp
    src/Gui/States/OptionsControls.cpp
    src/Gui/States/OptionsControlsKeys.cpp
    src/Gui/States/OptionsControlsKeyChangeWindow.cpp
    src/Gui/States/Pause.cpp
    src/Gui/States/Win.cpp
    src/Gui/States/NewGame.cpp
    src/MazeRenderers/Classic.cpp
    src/MazeRenderers/Gray.cpp
    src/MazeRenderers/Brick.cpp
    src/MazeRenderers/NightBrick.cpp)

set(HEADERS
    src/Camera.hpp
    src/Chunk.hpp
    src/Game.hpp
    src/GraphicEngine.hpp
    src/IRenderable.hpp
    src/ITickable.hpp
    src/Maze.hpp
    src/MazeRenderer.hpp
    src/Player.hpp
    src/StarSky.hpp
    src/Settings.hpp
    src/Saver.hpp
    src/FpsCalculator.hpp
    src/utils.hpp
    src/path_separator.hpp
    src/Skybox.hpp
    src/Logger.hpp
    src/Gui/Background.hpp
    src/Gui/MainMenu.hpp
    src/Gui/Gui.hpp
    src/Gui/State.hpp
    src/Gui/States/Hud.hpp
    src/Gui/States/Main.hpp
    src/Gui/States/OptionsMenu.hpp
    src/Gui/States/Options.hpp
    src/Gui/States/OptionsGraphics.hpp
    src/Gui/States/OptionsOther.hpp
    src/Gui/States/OptionsControls.hpp
    src/Gui/States/Pause.hpp
    src/Gui/States/Win.hpp
    src/Gui/States/NewGame.hpp
    src/MazeRenderers/Classic.hpp
    src/MazeRenderers/Gray.hpp
    src/MazeRenderers/Brick.hpp
    src/MazeRenderers/NightBrick.hpp)

if (WIN32)
    set(SOURCES ${SOURCES} win/resource.rc)
endif (WIN32)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
else (WIN32)
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif (WIN32)

if (SFGUI_FOUND)
    message(STATUS "Found SFGUI ${SFGUI_VERSION} in ${SFGUI_DIR}")
else (SFGUI_FOUND)
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

    target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/sfgui/include)
endif (SFGUI_FOUND)

set(JSONCPP_TARGET jsoncpp_lib_static)

get_target_property(JSON_INC_PATH ${JSONCPP_TARGET} INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(${PROJECT_NAME} PRIVATE ${JSON_INC_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${Intl_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CONFIG++_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE lib)

target_link_libraries(${PROJECT_NAME} ${JSONCPP_TARGET})
target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES})
target_link_libraries(${PROJECT_NAME} SFGUI::SFGUI)
target_link_libraries(${PROJECT_NAME} sfml-window sfml-graphics sfml-system)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} Threads::Threads)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} iconv)
endif (WIN32)

target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_NLS=1)

set(MAZEMAZE_INSTALL_DIR "/opt" CACHE PATH "Mazemaze Install directory")

configure_file(mazemaze.desktop.in mazemaze.desktop)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${MAZEMAZE_INSTALL_DIR}/${PROJECT_NAME})

install(DIRECTORY data
        DESTINATION ${MAZEMAZE_INSTALL_DIR}/${PROJECT_NAME})

install(FILES data/icon.png
        DESTINATION share/icons/hicolor/256x256/apps
        COMPONENT ICON
        RENAME mazemaze.png)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/mazemaze.desktop
        DESTINATION share/applications
        COMPONENT DESKTOP_ENTRY)

add_subdirectory(locale)
