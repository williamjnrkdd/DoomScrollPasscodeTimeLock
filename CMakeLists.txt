cmake_minimum_required(VERSION 3.20)

project(app LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# ============================================================
# libqrencode
# ============================================================

# Disable PNG requirement and CLI tools/tests in libqrencode
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_TESTS OFF CACHE BOOL "" FORCE)
set(WITHOUT_PNG ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    libqrencode
    GIT_REPOSITORY https://github.com/fukuchi/libqrencode.git
    GIT_TAG v4.1.1
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(libqrencode)


# ============================================================
# cJSON
# ============================================================

add_library(cjson STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cJSON/cJSON.c"
)

target_include_directories(cjson PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/cJSON"
)


# ============================================================
# libcurl
# ============================================================


FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_16_0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(curl)


# ============================================================
# SDL2
# ============================================================

find_package(SDL2 REQUIRED)

# ============================================================
# Application sources
# ============================================================

file(GLOB SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)


# ============================================================
# Executable
# ============================================================

add_executable(app ${SOURCES})

target_include_directories(app PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor"
    "${libqrencode_SOURCE_DIR}"
)


# Static libcurl on Windows requires CURL_STATICLIB.
target_compile_definitions(app PRIVATE
    CURL_STATICLIB
)


target_link_libraries(app PRIVATE
    qrencode
    cjson
    libcurl
    SDL2::SDL2
)


# ============================================================
# Development tools
# ============================================================

option(BUILD_TOOLS "Build development tools" OFF)

if(BUILD_TOOLS)
    add_executable(prime_generator
        helpers/prime_number_generator.c
    )
endif()