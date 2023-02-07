cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(MiniRenderer)

option(PLATFORM_WINDOWS "Build for Windows?" ON)

if(PLATFORM_WINDOWS)
    list(APPEND COMPILE_DEFS "PLATFORM_WINDOWS")
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/Platform/Windows")
    list(APPEND EXTRA_LIBS gdi32 user32)
else()
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src/Platform/Linux")
    list(APPEND EXTRA_LIBS X11)
endif()

set(SOURCE_FILES src/Core/Renderer.cpp src/Core/Renderer.h src/Core/Window.h
                 src/Core/Events/Event.h src/Core/Events/KeyEvents.h src/Core/Events/MouseEvents.h 
                 src/Core/Events/WindowEvents.h src/Core/Events/EventHandler.h src/Core/Events/EventHandler.cpp
                 src/Core/Framebuffer.cpp src/Core/Framebuffer.h
                 src/Core/Swapchain.cpp src/Core/Swapchain.h
                 src/Core/Maths.h
                 src/Core/LineRenderer.h
                 src/Core/TriangleRenderer.h
                 src/Core/Model.cpp src/Core/Model.h
                 src/Platform/Windows/WindowsWindow.h src/Platform/Windows/WindowsWindow.cpp
                 src/Platform/Linux/LinuxWindow.h src/Platform/Linux/LinuxWindow.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${COMPILE_DEFS} PUBLIC PROJECT_DIR="${PROJECT_SOURCE_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})