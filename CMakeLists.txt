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

# geode-sdk's own CMakeLists.txt include()s files using relative paths like
# "cmake/CPM.cmake". Because include() (unlike add_subdirectory()) does NOT
# change which folder relative paths resolve against, those paths actually
# get looked up inside OUR project folder, not the SDK's. So: copy the SDK's
# own cmake helper scripts into our project folder first, every build.
file(GLOB GEODE_SDK_CMAKE_HELPERS "$ENV{GEODE_SDK}/cmake/*.cmake")
if (GEODE_SDK_CMAKE_HELPERS)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    file(COPY ${GEODE_SDK_CMAKE_HELPERS} DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
endif()

# Belt-and-suspenders: if CPM.cmake still isn't there after that, grab it directly.
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")
    message(STATUS "CPM.cmake still missing, downloading it directly")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake"
    )
endif()

include($ENV{GEODE_SDK}/CMakeLists.txt)

setup_geode_mod(${PROJECT_NAME})
