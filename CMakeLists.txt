cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GlobedMobileVC VERSION 0.1.0)

file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define the GEODE_SDK environment variable to point to your Geode SDK install.")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# Some SDK checkouts are missing this helper script -- fetch it ourselves if so.
if (NOT EXISTS "$ENV{GEODE_SDK}/cmake/CPM.cmake")
    message(STATUS "CPM.cmake missing from Geode SDK, downloading it")
    file(MAKE_DIRECTORY "$ENV{GEODE_SDK}/cmake")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
        "$ENV{GEODE_SDK}/cmake/CPM.cmake"
    )
endif()

include($ENV{GEODE_SDK}/CMakeLists.txt)

setup_geode_mod(${PROJECT_NAME})
