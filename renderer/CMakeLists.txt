cmake_minimum_required(VERSION 3.13)
project(TJRenderer)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE cmake-build-debug)

set(HEADERS
        error.h
        geometry.h
        image.h
        model.h
        platform.h
        scene.h
        shader.h
        transformation.h
        triangle.h
        tgaimage.h
        )

set(SOURCES
        ambient_shader.cpp
        depth_shader.cpp
        error.cpp
        general_shader.cpp
        geometry.cpp
        image.cpp
        main.cpp
        model.cpp
        scene.cpp
        shadow_shader.cpp
        tgaimage.cpp
        transformation.cpp
        triangle.cpp
        )

if(WIN32)
    set(SOURCES ${SOURCES} win32.cpp)
elseif(APPLE)
    set(SOURCES ${SOURCES} macos.mm wireframe.cpp)
endif()

source_group(TREE "${CMAKE_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES})

set(TARGET Viewer)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

set_target_properties(${TARGET} PROPERTIES C_STANDARD 90)
set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS OFF)
set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4)
    target_compile_options(${TARGET} PRIVATE /fp:fast)
    target_compile_options(${TARGET} PRIVATE /D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(${TARGET} PRIVATE -ffast-math)
endif()

if(WIN32)
    # nothing to do for now
elseif(APPLE)
    target_link_libraries(${TARGET} PRIVATE "-framework Cocoa")
else()
    target_link_libraries(${TARGET} PRIVATE m X11)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")
