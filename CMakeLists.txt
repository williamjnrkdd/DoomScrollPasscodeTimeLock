cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

# zlib
FetchContent_Declare(
    zlib
    URL https://github.com/madler/zlib/archive/refs/tags/v1.3.1.tar.gz
)
FetchContent_MakeAvailable(zlib)


# libpng
FetchContent_Declare(
    libpng
    URL https://github.com/pnggroup/libpng/archive/refs/tags/v1.6.50.tar.gz
)
FetchContent_MakeAvailable(libpng)

if(NOT TARGET PNG::PNG)
    if(TARGET png_static)
        add_library(PNG::PNG ALIAS png_static)
    elseif(TARGET png_shared)
        add_library(PNG::PNG ALIAS png_shared)
    else()
        message(FATAL_ERROR "libpng was added but no png target was found")
    endif()
endif()


# libqrencode
FetchContent_Declare(
    libqrencode
    URL https://github.com/fukuchi/libqrencode/archive/refs/tags/v4.1.1.tar.gz
)
FetchContent_MakeAvailable(libqrencode)


# cJSON
add_library(cjson STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cJSON/cJSON.c"
)

target_include_directories(cjson PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cJSON"
)


# libcurl
find_package(CURL REQUIRED)


# --------------------------------------------------
# Application sources
# --------------------------------------------------

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)


# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(app ${SOURCES})


# Own headers
target_include_directories(app PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
    "${libqrencode_SOURCE_DIR}"
)


# --------------------------------------------------
# Libraries
# --------------------------------------------------

target_link_libraries(app PRIVATE
    qrencode
    PNG::PNG
    cjson
    CURL::libcurl
)

option(BUILD_TOOLS "Build development tools" OFF)

if(BUILD_TOOLS)
    add_executable(prime_generator
        helpers/prime_number_generator.c
    )
endif()