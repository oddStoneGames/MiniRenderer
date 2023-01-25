cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

project(MiniRenderer)

option(WINDOWS "Build for Windows?" ON)

if(WINDOWS)
    list(APPEND COMPILE_DEFS "WINDOWS")
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/Platform/Windows")
    list(APPEND EXTRA_LIBS gdi32 user32)
else()
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/Platform/Linux")
    list(APPEND EXTRA_LIBS X11)
endif()

set(SOURCE_FILES src/Core/Renderer.cpp src/Core/Renderer.h src/Core/Window.h
                 src/Core/Events/Event.h src/Core/Events/KeyEvents.h src/Core/Events/MouseEvents.h 
                 src/Core/Events/WindowEvents.h src/Core/Events/EventHandler.h src/Core/Events/EventHandler.cpp
                 src/Platform/Windows/WindowsWindow.h src/Platform/Windows/WindowsWindow.cpp
                 src/Platform/Linux/LinuxWindow.h src/Platform/Linux/LinuxWindow.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPILE_DEFS})
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})